/*! \file
 *
 * \brief Parallelization functionality (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 
#include<memory>
#include "bpmodule/parallel/InitFinalize.hpp"
#include "bpmodule/output/Output.hpp"
#include "bpmodule/util/Cmdline.hpp"
#include "LibTaskForce.hpp"

namespace bpmodule {
namespace parallel {

typedef LibTaskForce::Environment Env_t;
static std::unique_ptr<Env_t> Env_;

const Env_t& GetEnv(){return *Env_;}
 


void Init(void)
{
    output::Output("Calling MPI Init\n");
    //In theory this will allow the user to tweak number of threads
    //for the moment we set it to 0, which is a special value that means use
    //all the threads available.
    size_t NThreads=1;
    Env_=std::unique_ptr<Env_t>(new Env_t(NThreads));
    
    //madness::initialize(*(util::GetArgc()), *(util::GetArgv()));
    output::Output("Initialized Process %1% of %2%\n", GetProcID(), GetNProc());
}


void Finalize(void)
{
    output::Output("Finalizing Process %1% of %2%\n", GetProcID(), GetNProc());
    madness::finalize();
}


long GetProcID(void)
{
    return static_cast<long>(Env_->Comm().Rank());
}


long GetNProc(void)
{
    return static_cast<long>(Env_->Comm().NProcs());
}

} // close namespace parallel
} // close namespace bpmodule
