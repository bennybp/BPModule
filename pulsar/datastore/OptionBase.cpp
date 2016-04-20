/*! \file
 *
 * \brief Storage of options data (base class) (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "pulsar/datastore/OptionBase.hpp"

namespace pulsar{
namespace datastore {


OptionBase::OptionBase(const std::string & key,
                       bool required,
                       const std::string & help)
    : key_(key), required_(required), help_(help)
{ }

OptionBase::OptionBase(std::string && key,
                       bool required,
                       std::string && help)
    : key_(std::move(key)), required_(required), help_(std::move(help))
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


const std::string & OptionBase::Help(void) const noexcept
{
    return help_;
}


bool OptionBase::HasIssues(void) const
{
    return (GetIssues().size());
}


} //closing namespace datastore
} //closing namespace pulsar
