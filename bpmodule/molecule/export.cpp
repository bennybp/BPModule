/*! \file
 *
 * \brief Python exports for the molecule library
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include <boost/python/module.hpp>

#include "bpmodule/molecule/Molecule.hpp"

using namespace boost::python;


namespace bpmodule {
namespace molecule {
namespace export_python {

BOOST_PYTHON_MODULE(molecule)
{
}


} // close namespace export_python
} // close namespace molecule 
} // close namespace bpmodule

