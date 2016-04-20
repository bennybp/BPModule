/*! \file
 *
 * \brief Various test for options (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef PULSAR_GUARD_TESTING__TESTOPTIONS_HPP_
#define PULSAR_GUARD_TESTING__TESTOPTIONS_HPP_

#include <string>

#include "pulsar/datastore/OptionMap.hpp"
#include "pulsar/python/Convert.hpp"


namespace pulsar{
namespace testing {


template<typename T>
void TestOptionMap_Get(const datastore::OptionMap & opt, const std::string & key)
{
    // ignore return value
    opt.Get<T>(key);
}


template<typename T>
void TestOptionMap_Change(datastore::OptionMap & opt, const std::string & key, pybind11::object value)
{
    // ignore return value
    const T tmp = python::ConvertToCpp<T>(value);
    opt.Change<T>(key, tmp);
}



} // close namespace testing
} // close namespace pulsar



#endif
