/*! \file
 *
 * \brief Parallelization functionality (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 
#include <memory>
#include "pulsar/parallel/InitFinalize.hpp"
#include "pulsar/output/GlobalOutput.hpp"
#include "pulsar/util/Cmdline.hpp"
#include <LibTaskForce/LibTaskForce.hpp>

namespace pulsar{
namespace parallel {

//typedef LibTaskForce::Environment Env_t;
//static std::unique_ptr<Env_t> Env_;

//const Env_t& get_env(){return *Env_;}
 


void initialize(size_t NThreads)
{
    output::print_global_output("Calling MPI Init\n");
    //Env_=std::unique_ptr<Env_t>(new Env_t(NThreads));

    output::print_global_output("Initialized Process %? of %?\n", get_proc_id(), get_nproc());
}


void finalize(void)
{
    output::print_global_output("Finalizing Process %? of %?\n", get_proc_id(), get_nproc());
    //Env_.reset();
}


long get_proc_id(void)
{
    //return static_cast<long>(Env_->Comm().Rank());
    return 0;
}


long get_nproc(void)
{
    //return static_cast<long>(Env_->Comm().NProcs());
    return 0;
}

} // close namespace parallel
} // close namespace pulsar
