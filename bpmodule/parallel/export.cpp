// Parallelization
#include "bpmodule/parallel/Parallel.hpp"


#include <boost/python.hpp>

using namespace boost::python;


namespace bpmodule {
namespace parallel {
namespace export_python {

BOOST_PYTHON_MODULE(parallel)
{
    // Parallelization
    def("InitParallel", &parallel::InitParallel);
    def("FinalizeParallel", &parallel::FinalizeParallel);
}


} // close namespace export_python
} // close namespace parallel 
} // close namespace bpmodule
