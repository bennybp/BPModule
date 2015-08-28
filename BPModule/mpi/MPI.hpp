#include "BPModule/python_helper/BoostPython_fwd.hpp"

namespace bpy = boost::python;

namespace bpmodule {
namespace mpi {


void InitMPI(const bpy::list & argv);
void FinalizeMPI(void);

long GetProcID(void);
long GetNProc(void);


} // close namespace mpi
} // close namespace bpmodule
