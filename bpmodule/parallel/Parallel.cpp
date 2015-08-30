#include "bpmodule/output/Output.hpp"
#include "bpmodule/parallel/Parallel.hpp"

#ifdef BPMODULE_MPI
#include <mpi.h>
#endif

namespace bpmodule {
namespace parallel {

long GetProcID(void)
{
    #ifdef BPMODULE_MPI
      int p;
      MPI_Comm_rank(MPI_COMM_WORLD, &p);
      return static_cast<long>(p);
    #else
      return 0;
    #endif
}

long GetNProc(void)
{
    #ifdef BPMODULE_MPI
      int s;
      MPI_Comm_size(MPI_COMM_WORLD, &s);
      return static_cast<long>(s);
    #else
      return 1;
    #endif
}

} // close namespace parallel
} // close namespace bpmodule
