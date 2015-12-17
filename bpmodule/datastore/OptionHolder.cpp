/*! \file
 *
 * \brief Storage of generic data via (key, value) pair (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "bpmodule/python/Call.hpp"


#include "bpmodule/datastore/OptionHolder.hpp"
#include "bpmodule/datastore/OptionTypes.hpp"
#include "bpmodule/exception/OptionException.hpp"
#include "bpmodule/output/Output.hpp"

using namespace bpmodule::python;
using namespace bpmodule::exception;
using namespace bpmodule::util;


namespace bpmodule {
namespace datastore {



///////////////////////////////////////////////////
// These are the allowed types of OptionHolder
///////////////////////////////////////////////////
template class OptionHolder<OptionInt>;
template class OptionHolder<OptionFloat>;
template class OptionHolder<bool>;
template class OptionHolder<std::string>;
template class OptionHolder<std::vector<OptionInt>>;
template class OptionHolder<std::vector<OptionFloat>>;
template class OptionHolder<std::vector<bool>>;
template class OptionHolder<std::vector<std::string>>;






///////////////////////////////////////////
// Forward declarations of free functions
///////////////////////////////////////////
template<typename T>
static void PrintOption_(const OptionHolder<T> & oph);
template<typename T>
static void PrintOption_(const OptionHolder<std::vector<T>> & oph);



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
template<typename T>
static OptionIssues ValidatorWrapper_(pybind11::object valobj, const std::string & key, const OptionHolder<T> & value)
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
template<typename T>
OptionHolder<T>::OptionHolder(const std::string & key, PythonType pytype,
                              bool required, pybind11::object validator,
                              const std::string & help, T * def)
    : OptionBase(key, pytype, required, help), default_(def)
{
    validator_ = std::bind(ValidatorWrapper_<T>, validator, Key(), std::placeholders::_1);
}


template<typename T>
OptionHolder<T>::OptionHolder(const std::string & key, PythonType pytype,
                              bool required, pybind11::object validator,
                              const std::string & help)
    : OptionHolder(key, pytype, required, validator, help, nullptr)
{
}



template<typename T>
OptionHolder<T>::OptionHolder(const std::string & key, PythonType pytype,
                              bool required, pybind11::object validator,
                              const std::string & help, const T & def)
    : OptionHolder(key, pytype, required, validator, help, new T(def))
{
    if(required)
        throw OptionException("Default value supplied for required option", Key());

    // check the default using the validator
    OptionIssues iss = GetIssues();
    if(iss.size())
        throw OptionException("Default value for this option does not pass validation", Key(), "issues", iss);
}




template<typename T>
OptionHolder<T>::OptionHolder(const OptionHolder & oph)
    : OptionBase(oph),
      validator_(oph.validator_)
{
    if(oph.value_)
        value_ = std::unique_ptr<T>(new T(*oph.value_));
    if(oph.default_)
        default_ = std::unique_ptr<T>(new T(*oph.default_));
}



template<typename T>
void OptionHolder<T>::Change(const T & value)
{
    value_ = std::unique_ptr<T>(new T(value));
}



template<typename T>
const T & OptionHolder<T>::Get(void) const
{
    if(value_)
        return *value_;
    else if(default_)
        return *default_;
    else
        throw OptionException("Option does not have a value", Key());
}



template<typename T>
const T & OptionHolder<T>::GetDefault(void) const
{
    if(default_)
        return *default_;
    else
        throw OptionException("Option does not have a default", Key());
}


template<typename T>
OptionBasePtr OptionHolder<T>::Clone(void) const
{
    return OptionBasePtr(new OptionHolder<T>(*this));
}

template<typename T>
const std::type_info & OptionHolder<T>::TypeInfo(void) const noexcept
{
    return typeid(T);
}

template<typename T>
const char * OptionHolder<T>::Type(void) const noexcept
{
    return typeid(T).name();
}


template<typename T>
std::string OptionHolder<T>::DemangledType(void) const
{
    return util::DemangleCppType<T>();
}


template<typename T>
bool OptionHolder<T>::HasValue(void) const noexcept
{
    return bool(value_) || bool(default_);
}



template<typename T>
bool OptionHolder<T>::HasDefault(void) const noexcept
{
    return bool(default_);
}



template<typename T>
bool OptionHolder<T>::IsDefault(void) const
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


template<typename T>
void OptionHolder<T>::ResetToDefault(void) noexcept
{
    value_.reset();
}


template<typename T>
bool OptionHolder<T>::Compare(const OptionBase & rhs) const
{
    const OptionHolder<T> * op = dynamic_cast<const OptionHolder<T> *>(&rhs);

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


template<typename T>
void OptionHolder<T>::Print(void) const
{
    // call the free function
    PrintOption_(*this);
}


template<typename T>
OptionIssues OptionHolder<T>::GetIssues(void) const
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
template<typename T>
pybind11::object OptionHolder<T>::GetPy(void) const
{
    try {
        return ConvertToPy(Get());
    }
    catch(exception::GeneralException & ex)
    {
        throw OptionException(ex, Key(), "valuetype", DemangledType());
    }
}


template<typename T>
void OptionHolder<T>::ChangePy(pybind11::object obj)
{
    T val;

    try {
        val = ConvertToCpp<T>(obj);
    }
    catch(exception::GeneralException & ex)
    {
        throw OptionException(ex, Key(), "valuetype", DemangledType());
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
 *
 * \todo Printing of floating point values
 */
template<typename T>
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
template<typename T>
static void PrintOption_(const OptionHolder<T> & oph)
{
    std::string optline = FormatString("          %|1$-20|      %|2$-20|      %|3$-20|      %|4$-20|     %|5$-10|       %6%\n",
                                       oph.Key(),                                                         // name/key
                                       PythonTypeToStr(oph.PyType()),                                     // type
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
template<typename T>
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
                                    PythonTypeToStr(oph.PyType()),                                     // type
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









} // close namespace datastore
} // close namespace bpmodule

