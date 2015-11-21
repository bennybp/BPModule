/*! \file
 *
 * \brief Parallelization functionality (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include "bpmodule/parallel/Parallel.hpp"
#include "bpmodule/output/Output.hpp"
#include "bpmodule/util/Cmdline.hpp"

#include <mpi.h>

namespace bpmodule {
namespace parallel {

void InitParallel(void)
{
    output::Output("Calling MPI Init");
    MPI_Init(util::GetArgc(), util::GetArgv());
    output::Output("Initialized Process %1% of %2%\n", GetProcID(), GetNProc());
}


void FinalizeParallel(void)
{
    output::Output("Finalizing Process %1% of %2%\n", GetProcID(), GetNProc());

    MPI_Finalize();
}


long GetProcID(void)
{
    int p;
    MPI_Comm_rank(MPI_COMM_WORLD, &p);
    return static_cast<long>(p);
}


long GetNProc(void)
{
    int s;
    MPI_Comm_size(MPI_COMM_WORLD, &s);
    return static_cast<long>(s);
}

} // close namespace parallel
} // close namespace bpmodule
