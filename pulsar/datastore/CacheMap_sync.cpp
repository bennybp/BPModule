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
using namespace pulsar::util;
using namespace pulsar::output;
using namespace pulsar::datastore::detail;

// Commands that are sent through MPI
#define MM_SYNC_NACK     -1
#define MM_SYNC_ACK       0
#define MM_SYNC_PING      1
#define MM_SYNC_QUERY     2
#define MM_SYNC_ADD       3
#define MM_SYNC_DELETE    4
#define MM_SYNC_GET       5
#define MM_SYNC_STOP    100


namespace {

struct MetaData
{
    std::string type;
    bphash::HashValue hash;
    unsigned int policy;

    template<typename Archive>
    void serialize(Archive & ar)
    {
        ar(type, hash, policy);
    }
};


void send_int(int rank, int tag, int i)
{
    MPI_Send(&i, 1, MPI_INT, rank, tag, MPI_COMM_WORLD);
}

int recv_int(int rank, int tag)
{
    int r;
    MPI_Recv(&r, 1, MPI_INT, rank, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    return r;
}

void send_str(int rank, int tag, const std::string & s)
{
    MPI_Send(s.c_str(), s.size(), MPI_CHAR, rank, tag, MPI_COMM_WORLD);
}

std::string recv_str(int rank, int tag)
{
    MPI_Status stat;
    int size;

    MPI_Probe(rank, tag, MPI_COMM_WORLD, &stat);
    MPI_Get_count(&stat, MPI_CHAR, &size);

    std::vector<char> vkey(size+1);
    MPI_Recv(vkey.data(), size, MPI_CHAR, rank, tag,
             MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    vkey[size] = '\0';

    return std::string(vkey.data());
}

void send_data(int rank, int tag, const ByteArray & ba)
{
    MPI_Send(ba.data(), ba.size(), MPI_BYTE, rank, tag, MPI_COMM_WORLD);
}

ByteArray recv_data(int rank, int tag)
{
    MPI_Status stat;
    int size;

    MPI_Probe(rank, tag, MPI_COMM_WORLD, &stat);
    MPI_Get_count(&stat, MPI_CHAR, &size);

    ByteArray ba(size);
    MPI_Recv(ba.data(), size, MPI_BYTE, rank, tag,
             MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    return ba;
}

void send_ack(int rank, int tag)
{
    send_int(rank, tag, MM_SYNC_ACK);
}

void send_nack(int rank, int tag)
{
    send_int(rank, tag, MM_SYNC_NACK);
}


} // close anonymous namespace


namespace pulsar {
namespace datastore {


void CacheMap::start_sync(int tag)
{
    std::lock_guard<std::mutex> l(mutex_);

    MPI_Barrier(MPI_COMM_WORLD);
    if(sync_tag_ >= 0)
        return; // already running

    if(tag < 0)
        throw GeneralException("Attempting to use a negative tag number for the sync thread");

    sync_tag_ = tag;
    sync_thread_ = std::thread(&CacheMap::sync_thread_func_, this);

    // make sure all ranks are running their event loop
    MPI_Barrier(MPI_COMM_WORLD);
}

void CacheMap::stop_sync(void)
{
    MPI_Barrier(MPI_COMM_WORLD);
    std::lock_guard<std::mutex> l(sync_comm_mutex_);

    if(sync_tag_ < 0)
        return; // not running

    int my_rank = parallel::get_proc_id();

    send_int(my_rank, sync_tag_, MM_SYNC_STOP);
    send_str(my_rank, sync_tag_, "Called from CacheMap::stop_sync");
    sync_thread_.join();
    sync_tag_ = -1;
}


void CacheMap::sync_thread_func_(void)
{

    const int my_rank = parallel::get_proc_id();
    const int nproc = parallel::get_nproc();

    print_global_debug("Starting sync event loop for rank %? (using tag %?)\n",
                       my_rank, sync_tag_);


    // This keeps track of what ranks have what keys
    // This is only used for rank 0
    std::multimap<std::string, int> key_rank_map;

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
        
    print_global_debug("Rank %? has %?/%? available entries in its cache\n",
                       my_rank, my_cache_keys.size(), this->size());

    if(my_rank == 0)
    {
        for(const auto & it : my_cache_keys)
            key_rank_map.emplace(it, 0);

        // receive from all other ranks
        for(int src = 1; src < nproc; src++)
        {
            ByteArray ba = recv_data(src, sync_tag_);
            auto recv_keys = from_byte_array<std::set<std::string>>(ba);
            for(const auto & it : recv_keys)
                key_rank_map.emplace(it, src);
        }
    }
    else
    {
        auto ckey_dat = to_byte_array(my_cache_keys);
        send_data(0, sync_tag_, ckey_dat);
    }


    bool keep_running = true;
    while(keep_running)
    {
        // receive a command message
        MPI_Status stat;
        int cmd = 0;
        MPI_Recv(&cmd, 1, MPI_INT, MPI_ANY_SOURCE, sync_tag_, MPI_COMM_WORLD, &stat);
        const int src = stat.MPI_SOURCE; 
        const std::string msg = recv_str(src, sync_tag_);

        print_global_debug("Received command %? from rank %? with msg %?\n", cmd, src, msg);

        if(cmd == MM_SYNC_PING)
        {
            send_ack(src, sync_tag_+1);
        }
        else if(cmd == MM_SYNC_QUERY)
        {
            if(key_rank_map.count(msg))
            {
                auto range = key_rank_map.equal_range(msg);
                int r = range.first->second;
                send_int(src, sync_tag_+1, r);
            }
            else
            {
                send_nack(src, sync_tag_+1);
            }
        }
        else if(cmd == MM_SYNC_ADD)
        {
            print_global_debug("Received message to add %? from rank %?\n",
                               msg, src);
            key_rank_map.emplace(msg, src);
        }
        else if(cmd == MM_SYNC_DELETE)
        {
            print_global_debug("Received message to delete %? from rank %?\n",
                               msg, src);

            auto range = key_rank_map.equal_range(msg);
            for(auto it = range.first; it != range.second; ++it)
            {
                if(it->second == src)
                    key_rank_map.erase(it);
            }
        
        }
        else if(cmd == MM_SYNC_GET)
        {
            print_global_debug("Rank %? wants key %? from rank %?\n",
                               src, msg, my_rank);

            bool found = false;
            ByteArray ba;
            MetaData md;
            
            {
                std::lock_guard<std::mutex> l(mutex_);

                if(cmap_.count(msg))
                {
                    const CacheMapEntry_ & cme = cmap_.at(msg);
                    if(cme.value->is_serializable())
                    {
                        ba = cme.value->to_byte_array();
                        md = MetaData{cme.value->type(), {}, cme.policy}; 

                        if(cme.value->is_hashable())
                            md.hash == cme.value->my_hash();
                            
                        found = true;
                    }
                }
            } // unlocks the map

            if(found)
            {
                auto ba_meta = to_byte_array(md);
                send_ack(src, sync_tag_+1);
                send_data(src, sync_tag_+1, ba_meta);
                send_data(src, sync_tag_+1, ba);
            }
            else
            {
                send_nack(src, sync_tag_+1);
            }
        }
        else if(cmd == MM_SYNC_STOP)
        {
            print_global_debug("Recieved STOP command with msg: %?\n", msg);
            keep_running = false;
        }
    }

    print_global_debug("Sync event loop ended for rank %? (using tag %?)\n", my_rank, sync_tag_);
}

void CacheMap::notify_distcache_add_(const std::string & key)
{
    std::lock_guard<std::mutex> l(sync_comm_mutex_);
    send_int(0, sync_tag_, MM_SYNC_ADD);
    send_str(0, sync_tag_, key);
}

void CacheMap::notify_distcache_delete_(const std::string & key)
{
    std::lock_guard<std::mutex> l(sync_comm_mutex_);
    send_int(0, sync_tag_, MM_SYNC_DELETE);
    send_str(0, sync_tag_, key);
}


void CacheMap::obtain_from_distcache_(const std::string & key)
{
    // are we currently syncing?
    if(sync_tag_ < 0)
        return;

    print_global_debug("Looking to obtain %? from dist cache\n", key);

    ByteArray ba_data, ba_meta;

    {
        std::lock_guard<std::mutex> l(sync_comm_mutex_);

        // ask rank 0 for who I should ask for the data
        send_int(0, sync_tag_, MM_SYNC_QUERY);
        send_str(0, sync_tag_, key);
        int r = recv_int(0, sync_tag_+1);

        print_global_debug("Looking to get key %? from rank %?\n", key, r);

        if(r < 0)
            return;

        // get the info from the desired rank
        send_int(r, sync_tag_, MM_SYNC_GET);
        send_str(r, sync_tag_, key);

        int ack = recv_int(r, sync_tag_+1);
        if(ack == MM_SYNC_ACK)
        {
            ba_meta = recv_data(r, sync_tag_+1);
            ba_data = recv_data(r, sync_tag_+1);
        }
        else
            return;
    }

    MetaData md = from_byte_array<MetaData>(ba_meta);

    //! \todo helper function for this? happens in checkpointing too
    SerializedCacheData scd{std::move(ba_data), md.type, md.hash, md.policy};
    auto pscd = std::make_shared<SerializedCacheData>(std::move(scd));
    std::unique_ptr<SerializedDataHolder> sdh(new SerializedDataHolder(pscd));

    std::lock_guard<std::mutex> l(mutex_);
    set_(key, std::move(sdh), md.policy);
}



} // close namespace datastore
} // close namespace pulsar
