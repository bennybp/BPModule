/*! \file
 *
 * \brief Python exports for parallelization
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/parallel/Parallel.hpp"


#include <boost/python.hpp>

using namespace boost::python;


namespace bpmodule {
namespace parallel {
namespace export_python {

BOOST_PYTHON_MODULE(parallel)
{
    // Parallelization
    def("InitParallel", InitParallel);
    def("FinalizeParallel", FinalizeParallel);
}


} // close namespace export_python
} // close namespace parallel 
} // close namespace bpmodule

