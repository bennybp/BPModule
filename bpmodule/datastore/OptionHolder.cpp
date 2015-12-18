/*! \file
 *
 * \brief Storage of generic data via (key, value) pair (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "bpmodule/python/Call.hpp"
#include "bpmodule/python/Pybind11_stl.hpp"
#include "bpmodule/datastore/OptionHolder.hpp"
#include "bpmodule/datastore/OptionTypes.hpp"
#include "bpmodule/exception/OptionException.hpp"
#include "bpmodule/output/Output.hpp"

using namespace bpmodule::python;
using namespace bpmodule::exception;
using namespace bpmodule::util;


namespace bpmodule {
namespace datastore {


//! \todo Move to hpp file, then use extern template. Move conversion of arguments to here rather than OptionMap

///////////////////////////////////////////////////
// These are the allowed types of OptionHolder
///////////////////////////////////////////////////
template class OptionHolder<OptionType::Int>;
template class OptionHolder<OptionType::Float>;
template class OptionHolder<OptionType::Bool>;
template class OptionHolder<OptionType::String>;
template class OptionHolder<OptionType::ListInt>;
template class OptionHolder<OptionType::ListFloat>;
template class OptionHolder<OptionType::ListBool>;
template class OptionHolder<OptionType::ListString>;
template class OptionHolder<OptionType::SetInt>;
template class OptionHolder<OptionType::SetFloat>;
template class OptionHolder<OptionType::SetBool>;
template class OptionHolder<OptionType::SetString>;






////////////////////////////////////
// Wrapper around validator
////////////////////////////////////
/*! \brief A wrapper around a python validation function
 *
 * Will call the python function \p func and give it \p val as its only argument
 *
 * \throw bpmodule::exception::PythonCallException if there is a problem calling
 *        the python function or if the return type can't be converted
 *
 */
template<OptionType OPTTYPE>
static OptionIssues ValidatorWrapper_(pybind11::object valobj, const std::string & key, const OptionHolder<OPTTYPE> & value)
{
    try {
        if(!valobj || python::DeterminePyType(valobj) == python::PythonType::None)
            return {};
        return python::CallPyFuncAttr<OptionIssues>(valobj, "Validate", value.GetPy());
    }
    catch(const bpmodule::exception::GeneralException & ex)
    {
        throw OptionException(ex, key, "when", "Attempting to validate an option");
    }
}






///////////////////////////////////////////////////
// OptionHolder members
///////////////////////////////////////////////////
template<OptionType OPTTYPE>
OptionHolder<OPTTYPE>::OptionHolder(const std::string & key,
                              bool required, pybind11::object validator,
                              const std::string & help,
                              typename OptionHolder<OPTTYPE>::stored_type * def)
    : OptionBase(key, required, help), default_(def)
{
    validator_ = std::bind(ValidatorWrapper_<OPTTYPE>, validator, Key(), std::placeholders::_1);
}


template<OptionType OPTTYPE>
OptionHolder<OPTTYPE>::OptionHolder(const std::string & key,
                              bool required, pybind11::object validator,
                              const std::string & help)
    : OptionHolder(key, required, validator, help, nullptr)
{
}



template<OptionType OPTTYPE>
OptionHolder<OPTTYPE>::OptionHolder(const std::string & key,
                              bool required, pybind11::object validator,
                              const std::string & help,
                              const typename OptionHolder<OPTTYPE>::stored_type & def)
    : OptionHolder(key, required, validator, help, new stored_type(def))
{
    if(required)
        throw OptionException("Default value supplied for required option", Key());

    // check the default using the validator
    OptionIssues iss = GetIssues();
    if(iss.size())
        throw OptionException("Default value for this option does not pass validation", Key(), "issues", iss);
}


template<OptionType OPTTYPE>
OptionHolder<OPTTYPE>::OptionHolder(const std::string & key,
                              bool required, pybind11::object validator,
                              const std::string & help,
                              const pybind11::object & def)
    : OptionHolder(key, required, validator, help, python::ConvertToCpp<stored_type>(def))
{
}



template<OptionType OPTTYPE>
OptionHolder<OPTTYPE>::OptionHolder(const OptionHolder<OPTTYPE> & oph)
    : OptionBase(oph),
      validator_(oph.validator_)
{
    if(oph.value_)
        value_ = std::unique_ptr<stored_type>(new stored_type(*oph.value_));
    if(oph.default_)
        default_ = std::unique_ptr<stored_type>(new stored_type(*oph.default_));
}



template<OptionType OPTTYPE>
void OptionHolder<OPTTYPE>::Change(const typename OptionHolder<OPTTYPE>::stored_type & value)
{
    value_ = std::unique_ptr<stored_type>(new stored_type(value));
}



template<OptionType OPTTYPE>
const typename OptionHolder<OPTTYPE>::stored_type &
OptionHolder<OPTTYPE>::Get(void) const
{
    if(value_)
        return *value_;
    else if(default_)
        return *default_;
    else
        throw OptionException("Option does not have a value", Key());
}



template<OptionType OPTTYPE>
const typename OptionHolder<OPTTYPE>::stored_type &
OptionHolder<OPTTYPE>::GetDefault(void) const
{
    if(default_)
        return *default_;
    else
        throw OptionException("Option does not have a default", Key());
}


template<OptionType OPTTYPE>
OptionBasePtr OptionHolder<OPTTYPE>::Clone(void) const
{
    return OptionBasePtr(new OptionHolder<OPTTYPE>(*this));
}


template<OptionType OPTTYPE>
const char * OptionHolder<OPTTYPE>::Type(void) const noexcept
{
    return OptionTypeToString(OPTTYPE);
}


template<OptionType OPTTYPE>
bool OptionHolder<OPTTYPE>::HasValue(void) const noexcept
{
    return bool(value_) || bool(default_);
}



template<OptionType OPTTYPE>
bool OptionHolder<OPTTYPE>::HasDefault(void) const noexcept
{
    return bool(default_);
}



template<OptionType OPTTYPE>
bool OptionHolder<OPTTYPE>::IsDefault(void) const
{
    if(!value_ && default_)
        return true;
    else
    {
        // ignore compiler warnings about floating point equality
        PRAGMA_WARNING_PUSH
        PRAGMA_WARNING_IGNORE_FP_EQUALITY

        return value_ && default_ && (*value_ == *default_);

        PRAGMA_WARNING_POP
    }
}


template<OptionType OPTTYPE>
void OptionHolder<OPTTYPE>::ResetToDefault(void) noexcept
{
    value_.reset();
}


template<OptionType OPTTYPE>
bool OptionHolder<OPTTYPE>::Compare(const OptionBase & rhs) const
{
    const OptionHolder<OPTTYPE> * op = dynamic_cast<const OptionHolder<OPTTYPE> *>(&rhs);

    if(op == nullptr)
        return false;

    PRAGMA_WARNING_PUSH
    PRAGMA_WARNING_IGNORE_FP_EQUALITY

    if(this->HasValue() && op->HasValue())
        return (this->Get() == op->Get());
    else if( (!this->HasValue()) && (!op->HasValue()))
        return true; // neither have a value, so that's ok
    else
        return false; // one has a value, the other doesn't

    PRAGMA_WARNING_POP
}


template<OptionType OPTTYPE>
void OptionHolder<OPTTYPE>::Print(void) const
{
}


template<OptionType OPTTYPE>
OptionIssues OptionHolder<OPTTYPE>::GetIssues(void) const
{
    if(!IsSetIfRequired())
        return OptionIssues{"Option is not set, but is required"};
    else if(validator_)
        return validator_(*this);
    else
        return OptionIssues();
}



/////////////////////////////////////////
// Python-related functions
/////////////////////////////////////////
template<OptionType OPTTYPE>
pybind11::object OptionHolder<OPTTYPE>::GetPy(void) const
{
    try {
        return ConvertToPy(Get());
    }
    catch(exception::GeneralException & ex)
    {
        throw OptionException(ex, Key());
    }
}


template<OptionType OPTTYPE>
void OptionHolder<OPTTYPE>::ChangePy(pybind11::object obj)
{
    stored_type val;

    try {
        val = ConvertToCpp<stored_type>(obj);
    }
    catch(exception::GeneralException & ex)
    {
        throw OptionException(ex, Key());
    }

    Change(val);
}




/////////////////////////////////////////
// Printing functions (free functions)
/////////////////////////////////////////

//////////////////
// Helpers
//////////////////
#if 0
/*! \brief Converts an option value to a string
 *
 * \todo Printing of floating point values
 */
template<OptionType OPTTYPE>
static std::string OptToString_(const T & opt)
{
    return std::to_string(opt);
}

/*! \brief Converts an option value to a string
 *
 * \todo Printing of floating point values
 */
static std::string OptToString_(const std::string & opt)
{
    return opt;
}


/*! \brief Converts an option value to a string
 *
 * Overload for a bool type
 */
static std::string OptToString_(const bool & opt)
{
    return (opt ? "True" : "False");
}


/*! \brief Prints a line corresponding to an option
 *
 * Will print its key, type, etc. If there is a problem
 * with the option, the Error() output is used. If the
 * option is not the default, the Changed() output is used.
 */
template<OptionType OPTTYPE>
static void PrintOption_(const OptionHolder<OPTTYPE> & oph)
{
    std::string optline = FormatString("          %|1$-20|      %|2$-20|      %|3$-20|      %|4$-20|     %|5$-10|       %6%\n",
                                       oph.Key(),                                                         // name/key
                                       oph.Type(),                                     // type
                                       (oph.HasValue() ? OptToString_(oph.Get()) : "(none)"),             // value
                                       (oph.HasDefault() ? OptToString_(oph.GetDefault()) : "(none)"),    // default
                                       (oph.IsRequired() ? "True" : "False"),                             // required
                                       oph.Help());                                                       // help/description


    if(oph.HasIssues())
        output::Error(optline);

    else if(!oph.IsDefault())
        output::Changed(optline);
    else
        output::Output(optline);
}



/*! \brief Prints a line corresponding to an option
 *
 * Overload for a vector type
 *
 * Will print its key, type, etc. If there is a problem
 * with the option, the Error() output is used. If the
 * option is not the default, the Changed() output is used.
 */
template<OptionType OPTTYPE>
static void PrintOption_(const OptionHolder<std::vector<T>> & oph)
{
    size_t nrows = 0;

    std::string valstr, defstr;

    if(oph.HasValue())
    {
        auto valuevec = oph.Get();
        nrows = valuevec.size();

        if(valuevec.size() == 0)
            valstr = "(empty)";
        else
            valstr = OptToString_(valuevec[0]);
    }
    else
        valstr = "(none)";


    if(oph.HasDefault())
    {
        auto defvec = oph.GetDefault();
        nrows = std::max(nrows, defvec.size());

        if(defvec.size() == 0)
            defstr = "(empty)";
        else
            defstr = OptToString_(defvec[0]);
    }
    else
        defstr = "(none)";




    std::vector<std::string> optlines;
    optlines.push_back(FormatString("          %|1$-20|      %|2$-20|      %|3$-20|      %|4$-20|     %|5$-10|       %6%\n",
                                    oph.Key(),                                                         // name/key
                                    oph.Type(),                                     // type
                                    valstr,                                                            // value
                                    defstr,                                                            // default
                                    (oph.IsRequired() ? "True" : "False"),                             // required
                                    oph.Help()));                                                      // help/description


    // start at 1 since we did the first separately
    for(size_t i = 1; i < nrows; i++)
    {
        if(oph.HasValue())
        {
            auto valuevec = oph.Get();
            if(valuevec.size() > i)
                valstr = OptToString_(valuevec[i]);
        }


        if(oph.HasDefault())
        {
            auto defvec = oph.GetDefault();
            if(defvec.size() > i)
                defstr = OptToString_(defvec[i]);
        }

        optlines.push_back(FormatString("          %|1$-20|      %|2$-20|      %|3$-20|      %|4$-20|     %|5$-10|       %6%\n",
                                        "",              // name/key
                                        "",              // type
                                        valstr,          // value
                                        defstr,          // default
                                        "",              // required
                                        ""));            // help/description

    }

    for(const auto & it : optlines)
    {
        if(!oph.IsSetIfRequired())
            output::Error(it);
        else if(!oph.IsDefault())
            output::Changed(it);
        else
            output::Output(it);
    }


}
#endif


} // close namespace datastore
} // close namespace bpmodule

