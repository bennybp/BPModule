/*! \file
 *
 * \brief Storage of generic data (source) 
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include <cstring>
#include <vector>

#include "bpmodule/datastore/OptionBase.hpp"

namespace bpmodule {
namespace datastore {
namespace detail {


///////////////////////////////////////
// Allowed option types
///////////////////////////////////////
template bool OptionBase::IsType<int>(void) const noexcept;
template bool OptionBase::IsType<double>(void) const noexcept;
template bool OptionBase::IsType<bool>(void) const noexcept;
template bool OptionBase::IsType<std::string>(void) const noexcept;
template bool OptionBase::IsType<std::vector<int>>(void) const noexcept;
template bool OptionBase::IsType<std::vector<double>>(void) const noexcept;
template bool OptionBase::IsType<std::vector<bool>>(void) const noexcept;
template bool OptionBase::IsType<std::vector<std::string>>(void) const noexcept;



OptionBase::OptionBase(const std::string & key,
           bool required,
           python_helper::PythonType pytype,
           const std::string & help)
    : key_(key), required_(required), expert_(false), pytype_(pytype), help_(help)
{ }

const std::string & OptionBase::Key(void) const noexcept
{
    return key_;
}


bool OptionBase::IsRequired(void) const noexcept
{
    return required_;
}



bool OptionBase::IsExpert(void) const noexcept
{
    return expert_;
}



void OptionBase::SetExpert(bool expert) noexcept
{
    expert_ = expert;
} 


bool OptionBase::IsValid(void) const noexcept
{
    return HasValue() || HasDefault() || !IsRequired();
}


python_helper::PythonType OptionBase::PyType(void) const noexcept
{
    return pytype_;
}


const std::string & OptionBase::Help(void) const noexcept
{
    return help_;
}


template<typename U>
bool OptionBase::IsType(void) const noexcept
{
   return (strcmp(typeid(U).name(), Type()) == 0);
}


} //closing namespace detail
} //closing namespace datastore
} //closing namespace bpmodule
