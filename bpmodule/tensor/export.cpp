/*! \file
 *
 * \brief Python exports for the tensor library
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/list.hpp> // needed since Init takes lists
                                 //! \todo move python parsing stuff to here?

#include "bpmodule/tensor/InitFinalize.hpp"

using namespace boost::python;


namespace bpmodule {
namespace tensor {
namespace export_python {

BOOST_PYTHON_MODULE(tensor)
{
    def("Init", Init);
    def("Finalize", Finalize);
}


} // close namespace export_python
} // close namespace tensor
} // close namespace bpmodule

