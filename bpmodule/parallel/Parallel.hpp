#ifndef _GUARD_PARALLEL_HPP_
#define _GUARD_PARALLEL_HPP_

#include "bpmodule/python_helper/BoostPython_fwd.hpp"


namespace bpmodule {
namespace parallel {


void InitParallel(const boost::python::list & argv);
void FinalizeParallel(void);

long GetProcID(void);
long GetNProc(void);


} // close namespace parallel
} // close namespace bpmodule

#endif
