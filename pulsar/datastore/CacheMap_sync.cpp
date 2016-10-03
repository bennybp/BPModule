/*! \file
 *
 * \brief Main module database class (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "pulsar/datastore/CacheMap.hpp"
#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/parallel/Parallel.hpp"
#include "pulsar/output/GlobalOutput.hpp"
#include "pulsar/util/Serialization.hpp"

using namespace pulsar::exception;

// Commands that are sent through MPI
#define MM_SYNC_NACK     -1
#define MM_SYNC_ACK       0
#define MM_SYNC_PING      1
#define MM_SYNC_QUERY     2
#define MM_SYNC_ADD       3
#define MM_SYNC_DELETE    4
#define MM_SYNC_STOP    100


namespace pulsar {
namespace datastore {


void CacheMap::start_sync(int tag)
{
    MPI_Barrier(MPI_COMM_WORLD);
    if(sync_tag_ >= 0)
        return; // already running

    if(tag < 0)
        throw GeneralException("Attempting to use a negative tag number for the sync thread");

    sync_tag_ = tag;
    sync_thread_ = std::thread(&CacheMap::sync_thread_func_, this);
}

void CacheMap::stop_sync(void)
{
    MPI_Barrier(MPI_COMM_WORLD);

    if(sync_tag_ < 0)
        return; // not running

    int cmd = MM_SYNC_STOP;
    int ack;
    int my_rank = parallel::get_proc_id();

    MPI_Status stat;

    MPI_Send(&cmd, 1, MPI_INT, my_rank, sync_tag_, MPI_COMM_WORLD);
    MPI_Recv(&ack, 1, MPI_INT, my_rank, sync_tag_, MPI_COMM_WORLD, &stat);

    if(ack != MM_SYNC_ACK)
        throw GeneralException("Unknown ack received when attempting to stop thread", "ack", ack);

    sync_thread_.join();
    sync_tag_ = -1;
}


void CacheMap::sync_thread_func_(void)
{
    MPI_Status stat;
    int cmd;
    const int ack = MM_SYNC_ACK;
    const int nack = MM_SYNC_NACK;
    int my_rank = parallel::get_proc_id();
    int nproc = parallel::get_nproc();
    int size = 0; // size of a message
    std::vector<char> key;
    std::string key_str;

    bool keep_running = true;
    output::print_global_debug("Starting sync event loop for rank %? (using tag %?)\n", my_rank, sync_tag_);


    // This keeps track of what ranks have what keys
    // This is only used for rank 0
    std::multimap<std::string, int> cachedata_rank_map;

    // Initial set up: See what ranks have what cache data
    // Collect what cache keys I have
    std::set<std::string> my_cache_keys;

    {
        std::lock_guard<std::mutex> l(mutex_);
        for(const auto & it : cmap_)
        {
            if( (it.second.policy & DistributeGlobal) &&
                it.second.value->is_serializable())
                my_cache_keys.insert(it.first);
        }
    }
        
    output::print_global_debug("Rank %? has %?/%? available entries in its cache\n",
                               my_rank, my_cache_keys.size(), this->size());

    if(my_rank == 0)
    {
        for(const auto & it : my_cache_keys)
            cachedata_rank_map.emplace(it, 0);

        // receive from all other ranks
        for(int src = 1; src < nproc; src++)
        {
            MPI_Probe(src, sync_tag_, MPI_COMM_WORLD, &stat);
            MPI_Get_count(&stat, MPI_BYTE, &size);
            ByteArray ba_recv(size);
            MPI_Recv(ba_recv.data(), size, MPI_BYTE, src, sync_tag_,
                     MPI_COMM_WORLD, &stat);

            auto recv_keys = util::from_byte_array<std::set<std::string>>(ba_recv);
            for(const auto & it : recv_keys)
                cachedata_rank_map.emplace(it, src);
        }

        output::print_global_debug("Rank 0 has %? entries in its cache data -> rank map\n",
                                   cachedata_rank_map.size());
        for(const auto & it : cachedata_rank_map)
            output::print_global_debug("    - [%?]  [%?]\n", it.second, it.first);
    }
    else
    {
        auto ckey_dat = util::to_byte_array(my_cache_keys);
        size = static_cast<int>(ckey_dat.size());
        MPI_Send(ckey_dat.data(), size, MPI_BYTE, 0, sync_tag_, MPI_COMM_WORLD);
    }


    while(keep_running)
    {
        MPI_Recv(&cmd, 1, MPI_INT, MPI_ANY_SOURCE, sync_tag_, MPI_COMM_WORLD, &stat);
        int src = stat.MPI_SOURCE; 
        output::print_global_debug("Received command %? from rank %?\n", cmd, src);

        if(cmd == MM_SYNC_PING)
        {
            MPI_Send(&ack, 1, MPI_INT, src, sync_tag_, MPI_COMM_WORLD);
            keep_running = true;
        }
        else if(cmd == MM_SYNC_QUERY)
        {
            MPI_Probe(src, sync_tag_, MPI_COMM_WORLD, &stat);
            MPI_Get_count(&stat, MPI_CHAR, &size);
            key.resize(size);
            MPI_Recv(key.data(), size, MPI_CHAR, src, sync_tag_,
                     MPI_COMM_WORLD, &stat);
            key[size] = '\0';

            key_str = std::string(key.data());
            
            if(cachedata_rank_map.count(key_str))
            {
                auto range = cachedata_rank_map.equal_range(key_str);
                int r = range.first->second;
                MPI_Send(&r, 1, MPI_INT, src, sync_tag_, MPI_COMM_WORLD);
            }
            else
            {
                MPI_Send(&nack, 1, MPI_INT, src, sync_tag_, MPI_COMM_WORLD);
            }

            keep_running = true;
        }
        else if(cmd == MM_SYNC_ADD)
        {
            MPI_Probe(src, sync_tag_, MPI_COMM_WORLD, &stat);
            MPI_Get_count(&stat, MPI_CHAR, &size);
            key.resize(size);
            MPI_Recv(key.data(), size, MPI_CHAR, src, sync_tag_,
                     MPI_COMM_WORLD, &stat);
            key[size] = '\0';
            key_str = std::string(key.data());
            output::print_global_debug("Received message to add %? from rank %?\n", key_str, src);

            cachedata_rank_map.emplace(key_str, src);
        
        }
        else if(cmd == MM_SYNC_DELETE)
        {
            MPI_Probe(src, sync_tag_, MPI_COMM_WORLD, &stat);
            MPI_Get_count(&stat, MPI_CHAR, &size);
            key.resize(size);
            MPI_Recv(key.data(), size, MPI_CHAR, src, sync_tag_,
                     MPI_COMM_WORLD, &stat);
            key[size] = '\0';
            key_str = std::string(key.data());
            output::print_global_debug("Received message to delete %? from rank %?\n", key_str, src);

            
            auto range = cachedata_rank_map.equal_range(key_str);
            for(auto it = range.first; it != range.second; ++it)
            {
                if(it->second == src)
                    cachedata_rank_map.erase(it);
            }
        
        }
        else if(cmd == MM_SYNC_STOP)
        {
            MPI_Send(&ack, 1, MPI_INT, src, sync_tag_, MPI_COMM_WORLD);
            keep_running = false;
        }
    }

    output::print_global_debug("Sync event loop ended for rank %? (using tag %?)\n", my_rank, sync_tag_);
}

void CacheMap::notify_distcache_add_(const std::string & key)
{
    int cmd = MM_SYNC_ADD;
    MPI_Send(&cmd, 1, MPI_INT, 0, sync_tag_, MPI_COMM_WORLD);
    MPI_Send(key.c_str(), key.size(), MPI_CHAR, 0, sync_tag_, MPI_COMM_WORLD);
}

void CacheMap::notify_distcache_delete_(const std::string & key)
{
    int cmd = MM_SYNC_DELETE;
    MPI_Send(&cmd, 1, MPI_INT, 0, sync_tag_, MPI_COMM_WORLD);
    MPI_Send(key.c_str(), key.size(), MPI_CHAR, 0, sync_tag_, MPI_COMM_WORLD);
}


bool CacheMap::obtain_from_distcache_(const std::string & key)
{
    // are we currently syncing?
    if(sync_tag_ < 0)
        return false;

    output::print_global_debug("Looking to obtain %? from dist cache\n", key);
    int cmd = MM_SYNC_QUERY;
    int r = 0;

    // ask rank 0 where who I should ask for the data
    MPI_Send(&cmd, 1, MPI_INT, 0, sync_tag_, MPI_COMM_WORLD);
    MPI_Send(key.c_str(), key.size(), MPI_CHAR, 0, sync_tag_, MPI_COMM_WORLD);

    MPI_Status stat;
    MPI_Recv(&r, 1, MPI_INT, 0, sync_tag_, MPI_COMM_WORLD, &stat);
    if(r < 0)
        return false; // not found anywhere

    output::print_global_debug("Going to ask %? for key %?\n", r, key);
    return false;
}



} // close namespace datastore
} // close namespace pulsar
