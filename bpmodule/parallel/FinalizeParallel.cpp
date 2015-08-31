#include "bpmodule/output/Output.hpp"
#include "bpmodule/parallel/Parallel.hpp"
#include "bpmodule/tensor/Tensor.hpp"

#ifdef BPMODULE_MPI
#include <mpi.h>
#endif

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
