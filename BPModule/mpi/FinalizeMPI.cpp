#include "BPModule/core/Output.hpp"

#include <boost/python.hpp>
#include <mpi.h>

namespace out = bpmodule::output;
namespace bpy = boost::python;

namespace bpmodule {
namespace mpi {

void FinalizeMPI(void)
{
    MPI_Finalize();
}


} // close namespace mpi
} // close namespace bpmodule
