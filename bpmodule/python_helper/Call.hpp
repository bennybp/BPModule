/*! \file
 *
 * \brief Calling of python functions (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_CALL_HPP_
#define _GUARD_CALL_HPP_

#include <string>
#include <boost/python.hpp>


namespace bpmodule {
namespace python_helper {


/*! \brief Calls a python object
 *
 * Wraps exceptions in bmodule exceptions
 */
template<typename... Targs>
boost::python::object CallPyFunc(const boost::python::object & obj, Targs... Fargs)
{
    //! \todo checks, and wrap with bpmodule::exception
    //! \todo test with optionmap validators?
    return obj(Fargs...); 
}


} // close namespace python_helper
} // close namespace bpmodule

#endif
