/*! \file
 *
 * \brief Python exports for basis set
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include <boost/python/module.hpp>

#include "bpmodule/basisset/BasisSet.hpp"


using namespace boost::python;


namespace bpmodule {
namespace basisset {
namespace export_python {

BOOST_PYTHON_MODULE(basisset)
{
}


} // close namespace export_python
} // close namespace basisset 
} // close namespace bpmodule

