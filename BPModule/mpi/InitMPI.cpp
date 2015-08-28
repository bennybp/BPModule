#include "BPModule/core/Output.hpp"
#include "BPModule/mpi/MPI.hpp"

#include <cstring>

#include <boost/python.hpp>
#include <mpi.h>

namespace out = bpmodule::output;
namespace bpy = boost::python;

namespace bpmodule {
namespace mpi {

void InitMPI(const bpy::list & argv)
{
    //! \todo Check for buffer overflows

    int argc = bpy::extract<int>(argv.attr("__len__")());

    // copy argv
    // argv[argc] should always be NULL
    // see http://www.open-mpi.org/community/lists/users/2013/11/22955.php
    char ** argvtmp = new char*[argc+1];
    for(int i = 0; i < argc; i++)
    {
        std::string arg = bpy::extract<std::string>(argv[i]);
        size_t len = arg.size();
        argvtmp[i] = new char[len+1];
        strncpy(argvtmp[i], arg.c_str(), len+1);
    }
    argvtmp[argc] = NULL;
  
    out::Output("Calling MPI Init");
    MPI_Init(&argc, &argvtmp);

    out::Output("Initialized Process %1% of %2%\n", GetProcID(), GetNProc());

    for(int i = 0; i < argc; i++)
      delete [] argvtmp[i];
    delete [] argvtmp;
}


} // close namespace mpi
} // close namespace bpmodule
