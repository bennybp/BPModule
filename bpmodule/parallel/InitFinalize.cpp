/*! \file
 *
 * \brief Parallelization functionality (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include "bpmodule/parallel/InitFinalize.hpp"
#include "bpmodule/output/Output.hpp"
#include "bpmodule/util/Cmdline.hpp"

#include "LibParallel.hpp"



namespace bpmodule {
namespace parallel {

std::unique_ptr<LibTaskForce::Environment> Env_;    
    
void Init(void)
{
    output::Output("Calling MPI Init");
    //In theory this will allow the user to tweak number of threads
    //for the moment we set it to 0, which is a special value that means use
    //all the threads available.
    size_t NThreads=0;
    Env_=
      std::unique_ptr<LibTaskForce::Environment>(
            new LibTaskForce::Environment(NThreads)
      );
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
