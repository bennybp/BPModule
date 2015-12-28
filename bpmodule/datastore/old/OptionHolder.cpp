/*! \file
 *
 * \brief Storage of generic data (inner class)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */



#include "bpmodule/datastore/OptionHolder.hpp"
#include "bpmodule/python/Call.hpp"
#include "bpmodule/util/FormatString.hpp"
#include "bpmodule/output/Output.hpp"


using bpmodule::exception::OptionException;


namespace bpmodule {
namespace datastore {


static OptionIssues ValidatorWrapper_(pybind11::object valobj, const std::string & key, pybind11::object val)
{
    try {
        return python::CallPyFuncAttr<OptionIssues>(valobj, "Validate", val);
    }
    catch(const bpmodule::exception::GeneralException & ex)
    {
        throw OptionException(ex, key, "when", "Attempting to validate an option");
    }
}





OptionHolder::OptionHolder(const std::string & key, const pybind11::object & def,
                           const pybind11::object & validator, bool required,
                           const std::string & pytype,
                           const std::string & help)
    : key_(key), required_(required),
      pytype_(pytype), help_(help)
{
    // check for None types
    // and set as a null pointer
    if(python::DeterminePyType(def) != python::PythonType::None)
        default_ = std::unique_ptr<pybind11::object>(new pybind11::object(def));

    
    if(python::DeterminePyType(validator) != python::PythonType::None)
    {
        if(!python::HasCallableAttr(validator, "Validate"))
            throw OptionException("Given validator doesn't have a callable Validate function", key_);

        validator_ = std::bind(&ValidatorWrapper_, validator, key_, std::placeholders::_1);
    }
}


OptionHolder::OptionHolder(const OptionHolder & oph)
    : key_(oph.key_), required_(oph.required_),
      pytype_(oph.pytype_), help_(oph.help_),
      validator_(oph.validator_) 
    
{
    if(oph.value_)
      value_ = std::unique_ptr<pybind11::object>(new pybind11::object(python::DeepCopy(*oph.value_)));
    if(oph.default_)
      default_ = std::unique_ptr<pybind11::object>(new pybind11::object(python::DeepCopy(*oph.default_)));
}


const std::string & OptionHolder::PyType(void) const noexcept
{
    return pytype_;
}


bool OptionHolder::HasValue(void) const noexcept
{
    return (bool)value_ || (bool)default_;
}


bool OptionHolder::HasDefault(void) const noexcept
{
    return (bool)default_;
}


bool OptionHolder::IsDefault(void) const
{
    if(!value_ && !default_)
        return true;
    else if(!value_ && default_)
        return true; 
    else
        return value_ && default_ && python::Equal(*value_, *default_);
}


void OptionHolder::ResetToDefault(void)
{
    value_.reset();
}


const std::string & OptionHolder::Help(void) const noexcept
{
    return help_;
}


bool OptionHolder::HasIssues(void) const
{
    return GetIssues().size();
}


OptionIssues OptionHolder::GetIssues(void) const
{
    if(!IsSetIfRequired())
        return OptionIssues{"Option is not set, but is required"};
    else if(validator_ && HasValue())
        return validator_(Get<pybind11::object>());
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


////////////////////////////////
// PRINTING
////////////////////////////////
void OptionHolder::Print(void) const
{
    using util::FormatString;

    std::string valstr = HasValue() ? python::String(Get<pybind11::object>()) : "(none)";
    std::string defstr = HasDefault() ? python::String(GetDefault<pybind11::object>()) : "(none)";
    std::string optline = FormatString("          %|1$-20|      %|2$-20|      %|3$-20|      %|4$-20|     %|5$-10|       %6%\n",
                                       Key(),                              // name/key
                                       (PyType()),                         // type
                                       valstr,                                 // value
                                       defstr,                                 // default
                                       (IsRequired() ? "True" : "False"),  // required
                                       Help());                            // help/description


    if(HasIssues())
        output::Error(optline);

    else if(!IsDefault())
        output::Changed(optline);
    else
        output::Output(optline);

}



} //closing namespace datastore
} //closing namespace bpmodule


