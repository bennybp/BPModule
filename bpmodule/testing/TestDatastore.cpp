/*! \file
 *
 * \brief Tests construction of ModuleInfo via python (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include "bpmodule/testing/TestingBase.hpp"
#include "bpmodule/datastore/OptionMap.hpp"
#include "bpmodule/datastore/CalcData.hpp"

#include <boost/python.hpp>

using bpmodule::datastore::OptionMap;
using bpmodule::datastore::CalcData;

namespace bpmodule {
namespace testing {


int TestOptionMap_Construct(int itest, const std::string & desc, bool expected, const boost::python::dict & opt)
{
    return TestConstruct<OptionMap>(itest, desc, expected, opt);
}



int TestOptionMap_TestPy(int itest, const std::string & desc, bool expected,
                             const OptionMap & opmap, const std::string & key, const boost::python::object & obj)
{
    return true;
}


} // close namespace testing
} // close namespace bpmodule
