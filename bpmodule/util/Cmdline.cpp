/*! \file
 *
 * \brief Storage and retrieval of command line arguments (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include "bpmodule/output/Output.hpp"
#include <cstring>
#include "bpmodule/util/Cmdline.hpp"
#include "bpmodule/exception/GeneralException.hpp"

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

void SetCmdline(const std::vector<std::string> & argv)
{
    if(set_)
        ClearCmdline();

    argc_ = static_cast<int>(argv.size());
    bpmodule::output::Debug("Command line has %1% args\n", argc_);
    for(const auto & it : argv)
        bpmodule::output::Debug("   %1%\n", it);

    // copy argv
    // argv[argc] should always be NULL
    // see http://www.open-mpi.org/community/lists/users/2013/11/22955.php
    argv_ = new char*[argc_+1];

    for(size_t i = 0; i < argv.size(); i++)
    {
        const std::string & arg = argv[i];
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
