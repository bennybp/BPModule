/*! \file
 *
 * \brief Parallelization functionality (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include <iostream>
#include <memory>
#include <mpi.h>
#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/parallel/Parallel.hpp"
#include "pulsar/util/Cmdline.hpp"

namespace pulsar{
namespace parallel {

using Env_t=LibTaskForce::HybridEnv;
static std::unique_ptr<Env_t> Env_;
 
const Env_t& get_env(){return *Env_;}
 


void initialize(size_t NThreads)
{
    int provided;
    MPI_Init_thread(nullptr,nullptr,MPI_THREAD_MULTIPLE,&provided);

    if(provided!=MPI_THREAD_MULTIPLE)
        throw pulsar::exception::GeneralException("MPI does not support threading");

    Env_=std::unique_ptr<Env_t>(new Env_t(MPI_COMM_WORLD,NThreads));

    std::cout << "Initialized process " << get_proc_id() << " of " << get_nproc() << "\n";
}


void finalize(void)
{
    std::cout << "Finalizing process " << get_proc_id() << " of " << get_nproc() << "\n";

    //! \todo shouldn't be needed?
    MPI_Barrier(MPI_COMM_WORLD);
    Env_.reset();
}


long get_proc_id(void)
{
    return static_cast<long>(Env_->comm().rank());
}


long get_nproc(void)
{
    return static_cast<long>(Env_->comm().nprocs());
}


} // close namespace parallel
} // close namespace pulsar
