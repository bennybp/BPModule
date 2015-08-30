#include "bpmodule/output/Output.hpp"
#include "bpmodule/parallel/Parallel.hpp"
#include "bpmodule/tensor/Tensor.hpp"

#include <mpi.h>

namespace bpmodule {
namespace parallel {

void FinalizeParallel(void)
{
    output::Output("Finalizing Process %1% of %2%\n", GetProcID(), GetNProc());

    #ifdef BPMODULE_MPI
        MPI_Finalize();
    #endif
}


} // close namespace parallel
} // close namespace bpmodule
