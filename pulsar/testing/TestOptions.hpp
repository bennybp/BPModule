/*! \file
 *
 * \brief Various test for options (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef PULSAR_GUARD_TESTING__TESTOPTIONS_HPP_
#define PULSAR_GUARD_TESTING__TESTOPTIONS_HPP_

#include <string>

#include "pulsar/datastore/OptionMap.hpp"
#include "pulsar/util/PythonHelper.hpp"


namespace pulsar{

template<typename T>
void TestOptionMap_Get(const OptionMap & opt, const std::string & key)
{
    // ignore return value
    opt.get<T>(key);
}


template<typename T>
void TestOptionMap_Change(OptionMap & opt, const std::string & key, pybind11::object value)
{
    // ignore return value
    const T tmp = convert_to_cpp<T>(value);
    opt.change<T>(key, tmp);
}

} // close namespace pulsar



#endif
