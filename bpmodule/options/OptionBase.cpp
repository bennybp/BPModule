/*! \file
 *
 * \brief Storage of generic data (base class) (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "bpmodule/options/OptionBase.hpp"

namespace bpmodule {
namespace options {
namespace detail {


OptionBase::OptionBase(const std::string & key,
           bool required,
           python_helper::PythonType pytype,
           const std::string & validatordesc,
           const std::string & help)
    : key_(key), required_(required), expert_(false),
      pytype_(pytype), validatordesc_(validatordesc), help_(help)
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


const std::string & OptionBase::ValidatorDesc(void) const noexcept
{
    return validatordesc_;
}


} //closing namespace detail
} //closing namespace options
} //closing namespace bpmodule
