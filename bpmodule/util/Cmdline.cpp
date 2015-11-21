#include <bpmodule/python_helper/Convert.hpp>

namespace {
  int argc_;
  char ** argv_;
  bool set_ = false;
}

namespace bpmodule {
namespace util {


int * GetArgc(void)
{
    //! \todo check if set
    return &argc_;
}

char *** GetArgv(void)
{
    //! \todo check if set
    return &argv_;
}

void SetCmdline(const boost::python::list & argv)
{
    //! \todo check if set (or else memory leak...)
    //! \todo Check for buffer overflows


    argc_ = boost::python::extract<int>(argv.attr("__len__")());

    // copy argv
    // argv[argc] should always be NULL
    // see http://www.open-mpi.org/community/lists/users/2013/11/22955.php
    argv_ = new char*[argc_+1];
    for(int i = 0; i < argc_; i++)
    {
        std::string arg = python_helper::ConvertToCpp<std::string>(argv[i]);
        size_t len = arg.size();
        argv_[i] = new char[len+1];
        strncpy(argv_[i], arg.c_str(), len+1);
    }
    argv_[argc_] = NULL;
}

void ClearCmdline(void)
{
    for(int i = 0; i < argc_; i++)
        delete [] argv_[i];
    delete [] argv_;
}


} // close namespace util
} // close namespace bpmodule
