#ifndef BPMODULE_PARALLEL_HPP
#define BPMODULE_PARALLEL_HPP

#include "BPModule/python_helper/BoostPython_fwd.hpp"

namespace bpy = boost::python;

namespace bpmodule {
namespace parallel {


void InitParallel(const bpy::list & argv);
void FinalizeParallel(void);

long GetProcID(void);
long GetNProc(void);


} // close namespace parallel
} // close namespace bpmodule

#endif
