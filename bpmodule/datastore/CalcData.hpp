/*! \file
 *
 * \brief Storage of generic calculation data (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_CALCDATA_HPP_
#define _GUARD_CALCDATA_HPP_

#include "bpmodule/datastore/PropertyMap.hpp"

namespace bpmodule {
namespace datastore {


/*! \brief Class to hold arbitrary inputs and outputs
 *
 *  A CalcData object can hold arbitrary data types and is
 *  used primarily to pass information to and from modules.
 *  Access to the underlying data is NOT allowed, ensuring well-defined
 *  behavior when two modules are using the same underlying data.
 *
 *  This implements a somewhat copy-on-write scheme, where replacing
 *  the data will cause the data to be replaced only in a particular
 *  object, while other objects will still retain the original data.
 *  This is done through the use of std::shared_ptr.
 */
typedef PropertyMap<std::string> CalcData;






} // close namespace datastore
} // close namespace bpmodule



#endif

