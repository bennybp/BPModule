/*! \file
 *
 * \brief Python exports for math
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include <boost/python/module.hpp>
#include <boost/python/def.hpp>

#include "bpmodule/math/Factorial.hpp"

using namespace boost::python;

//! \todo Export exact casts? Or have the equivalent with python?

namespace bpmodule {
namespace math {
namespace export_python {

BOOST_PYTHON_MODULE(math)
{
    def("Factorial", Factorial); 
    def("FactorialF", FactorialF); 
    def("FactorialD", FactorialD); 

    def("DoubleFactorial", DoubleFactorial); 
    def("DoubleFactorialF", DoubleFactorialF); 
    def("DoubleFactorialD", DoubleFactorialD); 

    def("Double2nm1Factorial", Double2nm1Factorial); 
    def("Double2nm1FactorialF", Double2nm1FactorialF); 
    def("Double2nm1FactorialD", Double2nm1FactorialD); 
}


} // close namespace export_python
} // close namespace math 
} // close namespace bpmodule

