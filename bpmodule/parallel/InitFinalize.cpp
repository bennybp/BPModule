/*! \file
 *
 * \brief Parallelization functionality (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include "bpmodule/parallel/InitFinalize.hpp"
#include "bpmodule/output/Output.hpp"
#include "bpmodule/util/Cmdline.hpp"

#include <madness/world/world.h>

namespace bpmodule {
namespace parallel {

void Init(void)
{
    output::Output("Calling MPI Init");
    madness::initialize(*(util::GetArgc()), *(util::GetArgv()));
    output::Output("Initialized Process %1% of %2%\n", GetProcID(), GetNProc());
}


void Finalize(void)
{
    output::Output("Finalizing Process %1% of %2%\n", GetProcID(), GetNProc());
    madness::finalize();
}


long GetProcID(void)
{
    madness::World & w = madness::World::get_default();
    return static_cast<long>(w.rank());
}


long GetNProc(void)
{
    madness::World & w = madness::World::get_default();
    return static_cast<long>(w.nproc());
}

} // close namespace parallel
} // close namespace bpmodule
