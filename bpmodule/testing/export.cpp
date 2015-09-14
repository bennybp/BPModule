/*! \file
 *
 * \brief Python exports for testing the core library
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/testing/AllTests.hpp"

#include <boost/python.hpp>

using namespace boost::python;


namespace bpmodule {
namespace testing {
namespace export_python {

BOOST_PYTHON_MODULE(testing)
{
    // Parallelization
    def("TestModuleInfo", TestModuleInfo);
}


} // close namespace export_python
} // close namespace testing 
} // close namespace bpmodule

