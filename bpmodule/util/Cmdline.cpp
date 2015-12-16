/*! \file
 *
 * \brief Storage and retrieval of command line arguments (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include "bpmodule/output/Output.hpp"
#include <cstring>
#include "bpmodule/util/Cmdline.hpp"
#include "bpmodule/exception/GeneralException.hpp"
#include "bpmodule/python/Convert.hpp"

namespace {
  int argc_;
  char ** argv_;
  bool set_ = false;
}

namespace bpmodule {
namespace util {


int * GetArgc(void)
{
    if(!set_)
        throw exception::GeneralException("Command line has not been set! Definite developer error");

    return &argc_;
}

char *** GetArgv(void)
{
    if(!set_)
        throw exception::GeneralException("Command line has not been set! Definite developer error");

    return &argv_;
}

void SetCmdline(pybind11::object argv)
{
    if(set_)
        ClearCmdline();

    auto vargv = python::ConvertToCpp<std::vector<std::string>>(argv);
    argc_ = static_cast<int>(vargv.size());
    bpmodule::output::Debug("Command line has %1% args\n", argc_);
    for(const auto & it : vargv)
        bpmodule::output::Debug("   %1%\n", it);

    // copy argv
    // argv[argc] should always be NULL
    // see http://www.open-mpi.org/community/lists/users/2013/11/22955.php
    argv_ = new char*[argc_+1];

    for(size_t i = 0; i < vargv.size(); i++)
    {
        const std::string & arg = vargv[i];
        size_t len = arg.size();
        argv_[i] = new char[len+1];
        strncpy(argv_[i], arg.c_str(), len+1);
    }

    argv_[argc_] = NULL;

    set_ = true;
}


void ClearCmdline(void)
{
    if(set_)
    {
        for(int i = 0; i < argc_; i++)
            delete [] argv_[i];
        delete [] argv_;

        set_ = false;
    }
}


} // close namespace util
} // close namespace bpmodule
