/*! \file
 *
 * \brief Parallelization functionality (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include <iostream>
#include <memory>
#include <mpi.h>
#include "pulsar/exception/PulsarException.hpp"
#include "pulsar/parallel/Parallel.hpp"
#include "pulsar/util/Cmdline.hpp"

namespace pulsar{

using Env_t=LibTaskForce::HybridEnv;
static std::unique_ptr<Env_t> Env_;
 
const Env_t& get_env(){return *Env_;}
 


void parallel_initialize(size_t NThreads)
{
    int provided;
    MPI_Init_thread(nullptr,nullptr,MPI_THREAD_MULTIPLE,&provided);

    if(provided!=MPI_THREAD_MULTIPLE)
        throw pulsar::PulsarException("MPI does not support threading");

    Env_=std::unique_ptr<Env_t>(new Env_t(MPI_COMM_WORLD,NThreads));

    std::cout << "Initialized process " << get_proc_id() << " of " << get_nproc() << "\n";
}


void parallel_finalize(void)
{
    ////////////////////////////////////////////////////////
    // this may rarely be called if initialize hasn't been. But
    // it is possible
    ////////////////////////////////////////////////////////

    // WARNING: Note that we can't do anything with Env_, etc. This is being
    //          called from the dynamic module destructor, so those things might
    //          not be around
    //
    //          We should be able to safely reset the environment though. The unique_ptr
    //          should exist, but what it points to might not

    int flag;
    MPI_Initialized(&flag);
    if(flag)
    {
        // Pulsar called MPI_Init, so it has to call MPI_Finalize
        std::cout << "Finalizing pulsar MPI\n";
        Env_.reset();
        MPI_Finalize();
    }
    else
        std::cout << "Not finalizing, since we haven't been initialized\n";

}


long get_proc_id(void)
{
    return static_cast<long>(Env_->comm().rank());
}


long get_nproc(void)
{
    return static_cast<long>(Env_->comm().nprocs());
}

} // close namespace pulsar
