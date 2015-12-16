/*! \file
 *
 * \brief Storage of generic data (inner class)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */



#include "bpmodule/datastore/OptionHolder.hpp"
#include "bpmodule/python/Types.hpp"


namespace bpmodule {
namespace datastore {



OptionHolder::OptionHolder(const std::string & key, const pybind11::object & def,
             ValidatorFunc validator, bool required,
             const std::string & pytype,
             const std::string & help)
    : key_(key), required_(required),
      pytype_(pytype), help_(help),
      value_(pybind11::object()), default_(python::DeepCopy(def)), validator_(validator) 
{
}


OptionHolder::OptionHolder(const OptionHolder & oph)
    : key_(oph.key_), required_(oph.required_),
      pytype_(oph.pytype_), help_(oph.help_),
      value_(python::DeepCopy(oph.value_)),
      default_(python::DeepCopy(oph.default_)),
      validator_(oph.validator_) 
    
{
}


const std::string & OptionHolder::PyType(void) const noexcept
{
    return pytype_;
}


bool OptionHolder::HasValue(void) const noexcept
{
    return value_.ptr() != nullptr || default_.ptr() != nullptr;
}


bool OptionHolder::HasDefault(void) const noexcept
{
    return value_.ptr() != nullptr || default_.ptr() != nullptr;
}


bool OptionHolder::IsDefault(void) const
{
    if(value_.ptr() == nullptr && default_.ptr() == nullptr)
        return true;
    else
    {
        return value_.ptr() != nullptr && 
               default_.ptr() != nullptr &&
               python::Equal(value_, default_);
    }
}


void OptionHolder::ResetToDefault(void)
{
    value_ = pybind11::object();
}


const std::string & OptionHolder::Help(void) const noexcept
{
    return help_;
}


bool OptionHolder::HasIssues(void) const
{
    return GetIssues().size();
}


void OptionHolder::Print(void) const
{
    // todo
}


OptionIssues OptionHolder::GetIssues(void) const
{
    if(!IsSetIfRequired())
        return OptionIssues{"Option is not set, but is required"};
    else if(validator_)
        return validator_(*this);
    else
        return OptionIssues();
}


bool OptionHolder::Compare(const OptionHolder & rhs) const
{
    if(!HasValue() && !rhs.HasValue())
        return true;
    else if(HasValue() != rhs.HasValue())
        return false;
    else // they must both have a vlaue
        return python::Equal(Get<pybind11::object>(), rhs.Get<pybind11::object>());
}



const std::string & OptionHolder::Key(void) const noexcept
{
    return key_;
}



bool OptionHolder::IsRequired(void) const noexcept
{
    return required_;
}



bool OptionHolder::IsSetIfRequired(void) const
{
    return HasValue() || HasDefault() || !IsRequired();
}




} //closing namespace datastore
} //closing namespace bpmodule


