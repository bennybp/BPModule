/*! \file
 *
 * \brief Python exports for parallelization
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/list.hpp> // needed since InitParallel takes lists
                                 //! \todo move python parsing stuff to here?

#include "bpmodule/parallel/Parallel.hpp"

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

