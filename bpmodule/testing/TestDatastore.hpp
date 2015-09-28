/*! \file
 *
 * \brief Various test for datastore (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_TESTDATASTORE_HPP_
#define _GUARD_TESTDATASTORE_HPP_

#include <string>


////////////////////////
// Forward declarations
////////////////////////
namespace bpmodule {
namespace datastore {
class OptionMap;
}
}
////////////////////////
// End Forward declarations
////////////////////////



namespace bpmodule {
namespace testing {



/*! \brief Test construction of an OptionMap object
 *
 * \param [in] itest A test number
 * \param [in] desc Some description
 * \param [in] expected True if this is supoosed to succeed, false if it should
 *                      throw an exception
 * \param [in] opt A python dictionary to construct from
 *
 * \return 0 if the test succeeds, 1 if it fails
 */
int TestOptionMap_Construct(int itest, const std::string & desc, bool expected, const boost::python::dict & opt);



/*! \brief Test validating a python object
 *
 * \todo Exceptions and args
 */
int TestOptionMap_TestPy(int itest, const std::string & desc, bool expected,
                           const datastore::OptionMap & opmap, const std::string & key, const boost::python::object & obj);




} // close namespace testing
} // close namespace bpmodule



#endif
