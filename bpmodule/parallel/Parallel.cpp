/*! \file
 *
 * \brief Parallelization functionality (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include "bpmodule/output/Output.hpp"
#include "bpmodule/parallel/Parallel.hpp"
#include "bpmodule/python_helper/Convert.hpp"

#include <mpi.h>

namespace bpmodule {
namespace parallel {

void InitParallel(const boost::python::list & argv)
{
    //! \todo Check for buffer overflows

    int argc = boost::python::extract<int>(argv.attr("__len__")());

    // copy argv
    // argv[argc] should always be NULL
    // see http://www.open-mpi.org/community/lists/users/2013/11/22955.php
    char ** argvtmp = new char*[argc+1];
    for(int i = 0; i < argc; i++)
    {
        std::string arg = python_helper::ConvertToCpp<std::string>(argv[i]);
        size_t len = arg.size();
        argvtmp[i] = new char[len+1];
        strncpy(argvtmp[i], arg.c_str(), len+1);
    }
    argvtmp[argc] = NULL;
  
    output::Output("Calling MPI Init");
    MPI_Init(&argc, &argvtmp);

    for(int i = 0; i < argc; i++)
      delete [] argvtmp[i];
    delete [] argvtmp;


    output::Output("Initialized Process %1% of %2%\n", GetProcID(), GetNProc());
}


void FinalizeParallel(void)
{
    output::Output("Finalizing Process %1% of %2%\n", GetProcID(), GetNProc());

    MPI_Finalize();
}


long GetProcID(void)
{
    int p;
    MPI_Comm_rank(MPI_COMM_WORLD, &p);
    return static_cast<long>(p);
}


long GetNProc(void)
{
    int s;
    MPI_Comm_size(MPI_COMM_WORLD, &s);
    return static_cast<long>(s);
}

} // close namespace parallel
} // close namespace bpmodule
