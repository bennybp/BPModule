#include "bpmodule/tensor/Tensor.hpp"
#include "bpmodule/output/Output.hpp"

#include <boost/python.hpp>
#include <memory>

namespace {
  std::unique_ptr<CTF::World> world_;

}



namespace bpmodule {
namespace tensor {


void InitTensor(const boost::python::list & argv)
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

        world_ = std::unique_ptr<CTF::World>(new CTF::World(argc, argvtmp));
      
        for(int i = 0; i < argc; i++)
          delete [] argvtmp[i];
        delete [] argvtmp;

    #endif

}

void FinalizeTensor(void)
{
   world_.reset(); 
}

CTF::World & GetWorld(void)
{
    return *world_;
}


} // close namespace tensor
} // close namespace bpmodule
