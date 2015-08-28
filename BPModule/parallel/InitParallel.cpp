#include "BPModule/core/Output.hpp"
#include "BPModule/parallel/Parallel.hpp"
#include "BPModule/tensor/Tensor.hpp"

#include <cstring>

#include <boost/python.hpp>

#ifdef BPMODULE_MPI
#include <mpi.h>
#endif

namespace out = bpmodule::output;
namespace bpy = boost::python;

namespace bpmodule {
namespace parallel {


void InitParallel(const bpy::list & argv)
{
    #ifdef BPMODULE_MPI
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

        // Cyclops
        bpmodule::tensor::InitTensor(argc, argvtmp);


        for(int i = 0; i < argc; i++)
          delete [] argvtmp[i];
        delete [] argvtmp;

    #endif

    out::Output("Initialized Process %1% of %2%\n", GetProcID(), GetNProc());
}


} // close namespace parallel
} // close namespace bpmodule
