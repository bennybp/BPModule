#include "bpmodule/tensor/Tensor.hpp"
#include "bpmodule/output/Output.hpp"
#include "bpmodule/python_helper/Convert.hpp"

#include <memory>

#include <boost/python/list.hpp>

#include "bpmodule/tensor/Matrix.hpp"



namespace {

}



namespace bpmodule {
namespace tensor {


void Init(const boost::python::list & argv)
{
    //! \todo Check for buffer overflows
    //! \todo Duplicated code from parallel core module
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


    ///////////////////////////
    // Initialize matrix
    ///////////////////////////
    El::Initialize(argc, argvtmp); 



    for(int i = 0; i < argc; i++)
      delete [] argvtmp[i];
    delete [] argvtmp;
}

void Finalize(void)
{
    ///////////////////////////
    // Finalize matrix
    ///////////////////////////
    El::Finalize();
}


} // close namespace tensor
} // close namespace bpmodule
