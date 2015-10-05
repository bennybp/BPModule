/*! \file
 *
 * \brief Parallelization functionality (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/output/Output.hpp"
#include "bpmodule/parallel/Parallel.hpp"

#ifdef BPMODULE_MPI
#include <mpi.h>
#endif

namespace bpmodule {
namespace parallel {

//ignore the fact that argv is not used
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void InitParallel(const boost::python::list & argv)
{
    #ifdef BPMODULE_MPI
        //! \todo Check for buffer overflows

        int argc = boost::python::extract<int>(argv.attr("__len__")());

        // copy argv
        // argv[argc] should always be NULL
        // see http://www.open-mpi.org/community/lists/users/2013/11/22955.php
        char ** argvtmp = new char*[argc+1];
        for(int i = 0; i < argc; i++)
        {
            std::string arg = boost::python::extract<std::string>(argv[i]);
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

    #endif

    output::Output("Initialized Process %1% of %2%\n", GetProcID(), GetNProc());
}
#pragma GCC diagnostic pop


void FinalizeParallel(void)
{
    output::Output("Finalizing Process %1% of %2%\n", GetProcID(), GetNProc());

    #ifdef BPMODULE_MPI
        MPI_Finalize();
    #endif
}


long GetProcID(void)
{
    #ifdef BPMODULE_MPI
      int p;
      MPI_Comm_rank(MPI_COMM_WORLD, &p);
      return static_cast<long>(p);
    #else
      return 0;
    #endif
}


long GetNProc(void)
{
    #ifdef BPMODULE_MPI
      int s;
      MPI_Comm_size(MPI_COMM_WORLD, &s);
      return static_cast<long>(s);
    #else
      return 1;
    #endif
}

} // close namespace parallel
} // close namespace bpmodule
