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
#define MM_SYNC_PING 0
#define MM_SYNC_ACK 1
#define MM_SYNC_STOP 100


namespace pulsar {
namespace datastore {


void CacheMap::start_sync(int tag)
{
    if(sync_tag_ >= 0)
        return; // already running

    if(tag < 0)
        throw GeneralException("Attempting to use a negative tag number for the sync thread");

    sync_tag_ = tag;
    sync_thread_ = std::thread(&CacheMap::sync_thread_func_, this);
}

void CacheMap::stop_sync(void)
{
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
    int ack = MM_SYNC_ACK;
    int my_rank = parallel::get_proc_id();
    int nproc = parallel::get_nproc();

    bool keep_running = true;
    output::print_global_debug("Starting sync event loop for rank %? (using tag %?)\n", my_rank, sync_tag_);


    // This keeps track of what ranks have what keys
    // This is only used for rank 0
    std::multimap<std::string, int> cachedata_rank_map;

    // Initial set up: See what ranks have what cache data
    // Collect what cache keys I have
    std::set<std::string> my_cache_keys = get_keys();
    output::print_global_debug("Rank %? has %? entries in its cache\n",
                               my_rank, size());

    if(my_rank == 0)
    {
        for(const auto & it : my_cache_keys)
            cachedata_rank_map.emplace(it, 0);

        // receive from all other ranks
        for(int src = 1; src < nproc; src++)
        {
            int size = 0;
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
        int size = static_cast<int>(ckey_dat.size());
        MPI_Send(ckey_dat.data(), size, MPI_BYTE, 0, sync_tag_, MPI_COMM_WORLD);
    }


    while(keep_running)
    {
        MPI_Recv(&cmd, 1, MPI_INT, MPI_ANY_SOURCE, sync_tag_, MPI_COMM_WORLD, &stat);
        int src_rank = stat.MPI_SOURCE; 
        output::print_global_debug("Received command %? from rank %?\n", cmd, src_rank);

        switch(cmd)
        {
            case MM_SYNC_PING:
                MPI_Send(&ack, 1, MPI_INT, src_rank, sync_tag_, MPI_COMM_WORLD);
                keep_running = true;
                break;
            case MM_SYNC_STOP:
                MPI_Send(&ack, 1, MPI_INT, src_rank, sync_tag_, MPI_COMM_WORLD);
                keep_running = false;
                break;
        }
    }

    output::print_global_debug("Sync event loop ended for rank %? (using tag %?)\n", my_rank, sync_tag_);
}

} // close namespace datastore
} // close namespace pulsar
