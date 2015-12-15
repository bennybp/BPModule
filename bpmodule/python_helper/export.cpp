/*! \file
 *
 * \brief Python exports for python helper functions
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "bpmodule/python_helper/Pybind11.hpp"


namespace bpmodule {
namespace python_helper {
namespace export_python {


PYBIND11_PLUGIN(python_helper)
{
    pybind11::module m("python_helper", "Some helper functions for python stuff");

    return m.ptr();
}


} // close namespace export_python
} // close namespace python_helper
} // close namespace bpmodule

