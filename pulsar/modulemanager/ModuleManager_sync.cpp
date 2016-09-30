/*! \file
 *
 * \brief Main module database class (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "pulsar/modulemanager/ModuleManager.hpp"
#include "pulsar/modulemanager/SyncCommands.hpp"
#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/parallel/Parallel.hpp"
#include "pulsar/output/GlobalOutput.hpp"

using namespace pulsar::exception;


namespace pulsar{
namespace modulemanager {


void ModuleManager::start_sync_thread(int tag)
{
    if(sync_tag_ >= 0)
        return; // already running

    if(tag < 0)
        throw ModuleManagerException("Attempting to use a negative tag number for the sync thread");

    sync_tag_ = tag;
    sync_thread_ = std::thread(&ModuleManager::sync_thread_func_, this);
}

void ModuleManager::stop_sync_thread(void)
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
        throw ModuleManagerException("Unknown ack received when attempting to stop thread", "ack", ack);

    sync_thread_.join();
    sync_tag_ = -1;
}


void ModuleManager::sync_thread_func_(void)
{
    MPI_Status stat;
    int cmd;
    int ack = MM_SYNC_ACK;
    int my_rank = parallel::get_proc_id();

    bool keep_running = true;
    output::print_global_debug("Starting sync event loop for rank %? (using tag %?)\n", my_rank, sync_tag_);

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

} // close namespace modulemanager
} // close namespace pulsar
