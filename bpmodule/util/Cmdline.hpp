#ifndef _GUARD_CMDLINE_HPP_
#define _GUARD_CMDLINE_HPP_

#include "bpmodule/python_helper/BoostPython_fwd.hpp"


namespace bpmodule {
namespace util {


int * GetArgc(void);

char *** GetArgv(void);

void SetCmdline(const boost::python::list & argv);

void ClearCmdline(void);


} // close namespace util
} // close namespace bpmodule


#endif
