/*! \file
 *
 * \brief Storage of options data (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "bpmodule/datastore/OptionHolder.hpp"
#include "bpmodule/python/Call.hpp"
#include "bpmodule/exception/Exceptions.hpp"
#include "bpmodule/output/Output.hpp"

using namespace bpmodule::python;
using namespace bpmodule::exception;
using namespace bpmodule::util;


namespace bpmodule {
namespace datastore {


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
static OptionIssues ValidatorWrapper_(pybind11::object & valobj, const std::string & key, const OptionHolder<OPTTYPE> & value)
{
    try {
        if(!valobj || python::IsNone(valobj))
            return {};
        return python::CallPyFuncAttr<OptionIssues>(valobj, "Validate", value.GetPy());
    }
    catch(const std::exception & ex)
    {
        throw OptionException(ex, "optionkey", key, "when", "Attempting to validate an option");
    }
}






///////////////////////////////////////////////////
// OptionHolder members
///////////////////////////////////////////////////
template<OptionType OPTTYPE>
OptionHolder<OPTTYPE>::OptionHolder(const std::string & key,
                                    bool required, const pybind11::object & validator,
                                    const std::string & help,
                                    stored_type * def)
    : OptionBase(key, required, help), default_(def)
{
    validator_ = std::bind(ValidatorWrapper_<OPTTYPE>, validator, Key(), std::placeholders::_1);
}


template<OptionType OPTTYPE>
OptionHolder<OPTTYPE>::OptionHolder(const std::string & key,
                                    bool required, const pybind11::object & validator,
                                    const std::string & help)
    : OptionHolder(key, required, validator, help, nullptr)
{
}



template<OptionType OPTTYPE>
OptionHolder<OPTTYPE>::OptionHolder(const std::string & key,
                                    bool required, const pybind11::object & validator,
                                    const std::string & help,
                                    const stored_type & def)
    : OptionHolder(key, required, validator, help, new stored_type(def))
{
    if(required)
        throw OptionException("Default value supplied for required option", "optionkey", Key());

    // check the default using the validator
    OptionIssues iss = GetIssues();
    if(iss.size())
        throw OptionException("Default value for this option does not pass validation",
                              "optionkey", Key(), "issues", iss);
}


template<OptionType OPTTYPE>
OptionHolder<OPTTYPE>::OptionHolder(const std::string & key,
                                    bool required, const pybind11::object & validator,
                                    const std::string & help,
                                    const pybind11::object & def)
    : OptionHolder(key, required, validator, help, python::ConvertToCpp<stored_type>(def))
{
}



template<OptionType OPTTYPE>
OptionHolder<OPTTYPE>::OptionHolder(const OptionHolder & oph)
    : OptionBase(oph),
      validator_(oph.validator_)
{
    if(oph.value_)
        value_ = std::unique_ptr<stored_type>(new stored_type(*oph.value_));
    if(oph.default_)
        default_ = std::unique_ptr<stored_type>(new stored_type(*oph.default_));
}


template<OptionType OPTTYPE>
OptionHolder<OPTTYPE>::OptionHolder(std::string && key, bool required,
                                    std::string && help,
                                    std::unique_ptr<stored_type> && value,
                                    std::unique_ptr<stored_type> && def)
    : OptionBase(std::move(key), required, std::move(help)),
      value_(std::move(value)), default_(std::move(def))
{
}



template<OptionType OPTTYPE>
void OptionHolder<OPTTYPE>::Change(const stored_type & value)
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
        throw OptionException("Option does not have a value", "optionkey", Key());
}



template<OptionType OPTTYPE>
const typename OptionHolder<OPTTYPE>::stored_type &
OptionHolder<OPTTYPE>::GetDefault(void) const
{
    if(default_)
        return *default_;
    else
        throw OptionException("Option does not have a default", "optionkey", Key());
}


template<OptionType OPTTYPE>
OptionBasePtr OptionHolder<OPTTYPE>::Clone(void) const
{
    return OptionBasePtr(new OptionHolder<OPTTYPE>(*this));
}


template<OptionType OPTTYPE>
const char * OptionHolder<OPTTYPE>::TypeString(void) const noexcept
{
    return OptionTypeToString(OPTTYPE);
}

template<OptionType OPTTYPE>
OptionType OptionHolder<OPTTYPE>::Type(void) const noexcept
{
    return OPTTYPE;
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

    if(op == nullptr) // option of a different type
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
OptionIssues OptionHolder<OPTTYPE>::GetIssues(void) const
{
    if(!IsSetIfRequired())
        return OptionIssues{"Option is not set, but is required"};
    else if(validator_)
        return validator_(*this);
    else
        return OptionIssues();
}


template<OptionType OPTTYPE>
ByteArray OptionHolder<OPTTYPE>::ToByteArray(void) const
{
    util::MemoryArchive mar;
    mar.BeginSerialization();
    mar.Serialize(Key(), IsRequired(), Help(), value_, default_);
    mar.EndSerialization();
    return mar.ToByteArray();
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
    catch(std::exception & ex)
    {
        throw OptionException(ex, "optionkey", Key());
    }
}


template<OptionType OPTTYPE>
void OptionHolder<OPTTYPE>::ChangePy(const pybind11::object & obj)
{
    stored_type val;

    try {
        val = ConvertToCpp<stored_type>(obj);
    }
    catch(std::exception & ex)
    {
        throw OptionException(ex, "optionkey", Key());
    }

    Change(val);
}




/////////////////////////////////////////
// Printing functions (free functions)
/////////////////////////////////////////

//////////////////
// Helpers
//////////////////
/*! \brief Converts an option value to a string
 */
template<typename T>
static std::vector<std::string> OptToString_(const T & opt)
{
    return {FormatString("%-12.8?", opt)};
}

/*! \brief Converts an option value to a string
 */
static std::vector<std::string> OptToString_(const std::string & opt)
{
    return {opt};
}


/*! \brief Converts an option value to a string
 *
 * Overload for a bool type
 */
static std::vector<std::string> OptToString_(const bool & opt)
{
    return {(opt ? "True" : "False")};
}



template<typename T>
static std::vector<std::string> OptToString_(const std::vector<T> & opt)
{
    if(opt.size() == 0)
        return {"(empty)"};

    std::vector<std::string> r;
    for(const auto & it : opt)
    {
        std::vector<std::string> tmp = OptToString_(it);
        r.insert(r.end(), tmp.begin(), tmp.end());
    }
    return r;
}


template<typename T>
static std::vector<std::string> OptToString_(const std::set<T> & opt)
{
    if(opt.size() == 0)
        return {"(empty)"};

    std::vector<std::string> r;
    for(const auto & it : opt)
    {
        std::vector<std::string> tmp = OptToString_(it);
        r.insert(r.end(), tmp.begin(), tmp.end());
    }
    return r;
}


template<typename T, typename U>
static std::vector<std::string> OptToString_(const std::map<T, U> & opt)
{
    //! \todo write this (and probably clean up the rest)
    if(opt.size() == 0)
        return {"(empty)"};
    else
        return {"(map)"};
}

/*! \brief Prints a line corresponding to an option
 *
 * Will print its key, type, etc. If there is a problem
 * with the option, the Error() output is used. If the
 * option is not the default, the Changed() output is used.
 */
template<OptionType OPTTYPE>
void OptionHolder<OPTTYPE>::Print(std::ostream & os) const
{
    using namespace bpmodule::output;

    std::vector<std::string> val = {"(none)"};
    std::vector<std::string> def = {"(none)"};

    if(default_)
        def = OptToString_(*default_);

    if(value_)
        val = OptToString_(*value_);
    else if(default_)
        val = def;

    // pad out
    size_t m = std::max(val.size(), def.size());
    val.resize(m);
    def.resize(m);

    std::string req = (IsRequired() ? "True" : "False");

    // print the first line
    std::vector<std::string> optlines;
    optlines.push_back(FormatString("          %-20?      %-20?      %-20?      %-20?     %-10?       %?\n",
                                    Key(), TypeString(), val[0], def[0], req,  Help()));


    // now other lines
    for(size_t i = 1; i < m; i++)
    {
        optlines.push_back(FormatString("          %-20?      %-20?      %-20?      %-20?     %-10?       %?\n",
                                        "", "", val[i], def[i], "", ""));
    }
   
    // now actually print 
    for(const auto & it : optlines)
    {
        OutputType type = OutputType::Output;

        if(!IsSetIfRequired())
            type = OutputType::Error;
        else if(!IsDefault())
            type = OutputType::Changed;

        GeneralOutput(os, type, it);
    }
}


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

template class OptionHolder<OptionType::DictIntInt>;
template class OptionHolder<OptionType::DictIntFloat>;
template class OptionHolder<OptionType::DictIntBool>;
template class OptionHolder<OptionType::DictIntString>;

template class OptionHolder<OptionType::DictStringInt>;
template class OptionHolder<OptionType::DictStringFloat>;
template class OptionHolder<OptionType::DictStringBool>;
template class OptionHolder<OptionType::DictStringString>;



//! \todo make_unique in c++14
#define CASE_RETURN_OPTIONHOLDER(TYPE) \
    case OptionType::TYPE: \
         if(python::IsNone(def)) \
             return std::unique_ptr<OptionBase>(new OptionHolder<OptionType::TYPE>(key, required, validator, help));\
         else\
             return std::unique_ptr<OptionBase>(new OptionHolder<OptionType::TYPE>(key, required, validator, help, def));

std::unique_ptr<OptionBase>
CreateOptionHolder(std::string key, OptionType opttype, bool required,
                   const pybind11::object & validator, std::string help,
                   const pybind11::object & def)
{
    switch(opttype)
    {
        CASE_RETURN_OPTIONHOLDER(Int)
        CASE_RETURN_OPTIONHOLDER(Float)
        CASE_RETURN_OPTIONHOLDER(Bool)
        CASE_RETURN_OPTIONHOLDER(String)
        CASE_RETURN_OPTIONHOLDER(SetInt)
        CASE_RETURN_OPTIONHOLDER(SetFloat)
        CASE_RETURN_OPTIONHOLDER(SetBool)
        CASE_RETURN_OPTIONHOLDER(SetString)
        CASE_RETURN_OPTIONHOLDER(ListInt)
        CASE_RETURN_OPTIONHOLDER(ListFloat)
        CASE_RETURN_OPTIONHOLDER(ListBool)
        CASE_RETURN_OPTIONHOLDER(ListString)
        CASE_RETURN_OPTIONHOLDER(DictIntInt)
        CASE_RETURN_OPTIONHOLDER(DictIntFloat)
        CASE_RETURN_OPTIONHOLDER(DictIntBool)
        CASE_RETURN_OPTIONHOLDER(DictIntString)
        CASE_RETURN_OPTIONHOLDER(DictStringInt)
        CASE_RETURN_OPTIONHOLDER(DictStringFloat)
        CASE_RETURN_OPTIONHOLDER(DictStringBool)
        CASE_RETURN_OPTIONHOLDER(DictStringString)
        default:
            throw std::logic_error("Unhandled option type");
    }
}

#undef CASE_RETURN_OPTIONHOLDER


//! \todo make_unique in c++14
#define CASE_UNSERIALIZE_OPTIONHOLDER(TYPE) \
    case OptionType::TYPE: {\
        std::unique_ptr<OptionHolder<OptionType::TYPE>::stored_type> value, def; \
        mar.Unserialize(key, required, help, value, def); \
        return std::unique_ptr<OptionBase>(new OptionHolder<OptionType::TYPE>\
                    (std::move(key), required, std::move(help), std::move(value), std::move(def)));\
    }

std::unique_ptr<OptionBase>
OptionHolderFromByteArray(OptionType opttype, const ByteArray & ba)
{
    MemoryArchive mar;
    mar.FromByteArray(ba);
    std::string key, help;
    bool required;

    mar.BeginUnserialization();

    switch(opttype)
    {
        CASE_UNSERIALIZE_OPTIONHOLDER(Int)
        CASE_UNSERIALIZE_OPTIONHOLDER(Float)
        CASE_UNSERIALIZE_OPTIONHOLDER(Bool)
        CASE_UNSERIALIZE_OPTIONHOLDER(String)
        CASE_UNSERIALIZE_OPTIONHOLDER(SetInt)
        CASE_UNSERIALIZE_OPTIONHOLDER(SetFloat)
        CASE_UNSERIALIZE_OPTIONHOLDER(SetBool)
        CASE_UNSERIALIZE_OPTIONHOLDER(SetString)
        CASE_UNSERIALIZE_OPTIONHOLDER(ListInt)
        CASE_UNSERIALIZE_OPTIONHOLDER(ListFloat)
        CASE_UNSERIALIZE_OPTIONHOLDER(ListBool)
        CASE_UNSERIALIZE_OPTIONHOLDER(ListString)
        CASE_UNSERIALIZE_OPTIONHOLDER(DictIntInt)
        CASE_UNSERIALIZE_OPTIONHOLDER(DictIntFloat)
        CASE_UNSERIALIZE_OPTIONHOLDER(DictIntBool)
        CASE_UNSERIALIZE_OPTIONHOLDER(DictIntString)
        CASE_UNSERIALIZE_OPTIONHOLDER(DictStringInt)
        CASE_UNSERIALIZE_OPTIONHOLDER(DictStringFloat)
        CASE_UNSERIALIZE_OPTIONHOLDER(DictStringBool)
        CASE_UNSERIALIZE_OPTIONHOLDER(DictStringString)
        default:
            throw std::logic_error("Unhandled option type");
    }
}

#undef CASE_UNSERIALIZE_OPTIONHOLDER



} // close namespace datastore
} // close namespace bpmodule

