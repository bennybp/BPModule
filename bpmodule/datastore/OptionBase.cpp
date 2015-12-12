/*! \file
 *
 * \brief Storage of generic data (base class) (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "bpmodule/datastore/OptionBase.hpp"

namespace bpmodule {
namespace datastore {
namespace detail {


OptionBase::OptionBase(const std::string & key,
           bool required,
           python_helper::PythonType pytype,
           const std::string & help)
    : key_(key), required_(required),
      pytype_(pytype), help_(help)
{ }

const std::string & OptionBase::Key(void) const noexcept
{
    return key_;
}


bool OptionBase::IsRequired(void) const noexcept
{
    return required_;
}



bool OptionBase::IsSetIfRequired(void) const noexcept
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


bool OptionBase::HasIssues(void) const
{
    return (GetIssues().size());
}



} //closing namespace detail
} //closing namespace datastore
} //closing namespace bpmodule
