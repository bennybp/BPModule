#include "BPModule/core/Output.hpp"
#include "BPModule/parallel/Parallel.hpp"

#include <mpi.h>

namespace out = bpmodule::output;

namespace bpmodule {
namespace parallel {

void FinalizeParallel(void)
{
    out::Output("Finalizing Process %1% of %2%\n", GetProcID(), GetNProc());

    #ifdef BPMODULE_MPI
        MPI_Finalize();
    #endif
}


} // close namespace parallel
} // close namespace bpmodule
