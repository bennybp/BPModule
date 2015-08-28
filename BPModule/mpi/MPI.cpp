#include "BPModule/core/Output.hpp"
#include "BPModule/mpi/MPI.hpp"

#include <mpi.h>

namespace out = bpmodule::output;
namespace bpy = boost::python;

namespace bpmodule {
namespace mpi {

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

} // close namespace mpi
} // close namespace bpmodule
