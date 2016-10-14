/*! \file
 *
 * \brief Storage and retrieval of command line arguments (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include <cstring>

#include "pulsar/util/Cmdline.hpp"
#include "pulsar/exception/Exceptions.hpp"


namespace {
  int argc_;
  char ** argv_;
  bool set_ = false;
}

namespace pulsar{
namespace util {


int * get_argc(void)
{
    if(!set_)
        throw GeneralException("Command line has not been set! Definite developer error");

    return &argc_;
}

char *** get_argv(void)
{
    if(!set_)
        throw GeneralException("Command line has not been set! Definite developer error");

    return &argv_;
}

void set_cmdline(const std::vector<std::string> & argv)
{
    if(set_)
        clear_cmdline();

    argc_ = static_cast<int>(argv.size());

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


void clear_cmdline(void)
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
} // close namespace pulsar
