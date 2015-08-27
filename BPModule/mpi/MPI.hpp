#include "BPModule/core/BoostPython_fwd.hpp"

namespace bpy = boost::python;

namespace bpmodule {
namespace mpi {


void InitMPI(const bpy::list & argv);
void FinalizeMPI(void);



} // close namespace mpi
} // close namespace bpmodule
