/*! \file
 *
 * \brief Storage of generic data via (key, value) pair (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include <boost/python.hpp>

#include "bpmodule/datastore/OptionHolder.hpp"
#include "bpmodule/python_helper/Convert.hpp"
#include "bpmodule/exception/OptionException.hpp"
#include "bpmodule/output/Output.hpp"
#include "bpmodule/output/FormatStr.hpp"

using bpmodule::python_helper::PythonType;
using bpmodule::python_helper::ConvertToCpp;
using bpmodule::python_helper::TestConvertToCpp;
using bpmodule::python_helper::DetermineType;
using bpmodule::python_helper::GetPyClass;
using bpmodule::python_helper::StrToPythonType;
using bpmodule::python_helper::PythonTypeToStr;
using bpmodule::exception::OptionException;


namespace bpmodule {
namespace datastore {
namespace detail {



///////////////////////////////////////////////////
// These are the allowed types of OptionHolder
///////////////////////////////////////////////////
template class OptionHolder<int>;
template class OptionHolder<double>;
template class OptionHolder<bool>;
template class OptionHolder<std::string>;
template class OptionHolder<std::vector<int>>;
template class OptionHolder<std::vector<double>>;
template class OptionHolder<std::vector<bool>>;
template class OptionHolder<std::vector<std::string>>;






///////////////////////////////////////////
// Forward declarations of free functions
///////////////////////////////////////////
template<typename T>
static void PrintOption_(const OptionHolder<T> & oph);
template<typename T>
static void PrintOption_(const OptionHolder<std::vector<T>> & oph);












///////////////////////////////////////////////////
// OptionHolder members
///////////////////////////////////////////////////
template<typename T>
OptionHolder<T>::OptionHolder(const std::string & key, T * def,
                              ValidatorFunc validator, bool required,
                              python_helper::PythonType pytype,
                              const std::string & help)
    : OptionBase(key, required, pytype, help),
      default_(def),
      validator_(validator)
{
    // check the default using the validator
    if(def != nullptr)
        ValidateOrThrow_(*default_, "initial default");
    if(def != nullptr && required)
        throw exception::OptionException("Default value supplied for required option", Key()); 
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
    ValidateOrThrow_(value, "new value");
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
        throw exception::OptionException("Option does not have a value", Key());
}



template<typename T>
const T & OptionHolder<T>::GetDefault(void) const
{
    if(default_)
        return *default_;
    else
        throw exception::OptionException("Option does not have a default", Key());
}


template<typename T>
OptionBase * OptionHolder<T>::Clone(void) const
{
    return new OptionHolder<T>(*this);
}


template<typename T>
constexpr const char * OptionHolder<T>::Type(void) const noexcept
{
    return typeid(T).name();
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
        return value_ && default_ && (*value_ == *default_);
}


template<typename T>
void OptionHolder<T>::ResetToDefault(void) noexcept
{
    value_.reset();
}


template<typename T>
void OptionHolder<T>::Print(void) const
{
    PrintOption_(*this);
}




template<typename T>
void OptionHolder<T>::ValidateOrThrow_(const T & value, const std::string & desc) const
{
    if(!validator_(value))
    {
        //! \todo add exception info from validator?
        if(!OptionBase::IsExpert())
            throw exception::OptionException("Value is not valid for this option", Key());
        else
            output::Warning("Value for option %1% \"%2\" is invalid. Ignoring\n", desc, Key());
    }
}





/////////////////////////////////////////
// Python-related functions
/////////////////////////////////////////
template<typename T>
boost::python::object OptionHolder<T>::GetPy(void) const
{
    if(!python_helper::TestConvertToPy(Get()))
        throw exception::OptionException("Cannot convert option value to python object", Key(),
                                         "valuetype", Type());

    return python_helper::ConvertToPy(Get());
}


template<typename T>
void OptionHolder<T>::ChangePy(const boost::python::object & obj)
{
    if(!python_helper::TestConvertToCpp<T>(obj))
        throw exception::OptionException("Cannot convert python object to option value", Key(),
                                         "valuetype", Type(),
                                         "pythontype", python_helper::GetPyClass(obj));

    // will validate inside Change()
    Change(python_helper::ConvertToCpp<T>(obj));
}




/////////////////////////////////////////
// Printing functions (free functions)
/////////////////////////////////////////

//////////////////
// Helpers
//////////////////
template<typename T>
static std::string OptToString_(const T & opt)
{
    return std::to_string(opt);
}

static std::string OptToString_(const std::string & opt)
{
    return opt;
}


static std::string OptToString_(const bool & opt)
{
    return (opt ? "True" : "False");
}



template<typename T>
static void PrintOption_(const OptionHolder<T> & oph)
{
    std::string optline = FormatStr("          %|1$-20|      %|2$-20|      %|3$-20|      %|4$-20|     %|5$-10|       %6%\n",
                                    oph.Key(),                                                         // name/key
                                    python_helper::PythonTypeToStr(oph.PyType()),                      // type
                                    (oph.HasValue() ? OptToString_(oph.Get()) : "(none)"),             // value
                                    (oph.HasDefault() ? OptToString_(oph.GetDefault()) : "(none)"),    // default
                                    (oph.IsRequired() ? "True" : "False"),                             // required
                                    oph.Help());                                                       // help/description


    if(!oph.IsValid())
        output::Error(optline);
    
    if(!oph.IsDefault())
        output::Changed(optline);
    else
        output::Output(optline);
}



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
    optlines.push_back(FormatStr("          %|1$-20|      %|2$-20|      %|3$-20|      %|4$-20|     %|5$-10|       %6%\n",
                                 oph.Key(),                                                         // name/key
                                 python_helper::PythonTypeToStr(oph.PyType()),                      // type
                                 valstr,                                                            // value
                                 defstr,                                                            // default
                                 (oph.IsRequired() ? "True" : "False"),                             // required
                                 oph.Help()));                                                      // help/description


    // start at 1 since we did the first separately
    for(size_t i = 1; i < nrows; i++)
    {
        std::string valstr, defstr;

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

        optlines.push_back(FormatStr("          %|1$-20|      %|2$-20|      %|3$-20|      %|4$-20|     %|5$-10|       %6%\n",
                                     "",              // name/key
                                     "",              // type
                                     valstr,          // value
                                     defstr,          // default
                                     "",              // required
                                     ""));            // help/description
        
    }

    for(const auto & it : optlines) 
    {
        if(!oph.IsValid())
            output::Error(it);
        else if(!oph.IsDefault())
            output::Changed(it);
        else
            output::Output(it);
    }


}






////////////////////////////////////////////////////////
// CreateOptionHolder & helper functions
////////////////////////////////////////////////////////
template<typename T>
bool ValidateWrapper(const boost::python::object & val, T arg)
{
    return boost::python::extract<bool>(val.attr("Validate")(arg));
}


template<typename T>
bool EmptyValidator(T arg)
{
    return true;
}


template<typename T>
OptionBasePtr CreateOptionHolder(const std::string & key, const boost::python::tuple & tup)
{
    PythonType ptype_default = DetermineType(tup[1]);

    T * def = nullptr;

    if(ptype_default != PythonType::None)
    {
        if(!TestConvertToCpp<T>(tup[1]))
            throw OptionException("Default for option cannot be converted from python",
                                  key,
                                  "fromtype", GetPyClass(tup[1]),
                                  "totype", typeid(T).name());

        // shouldn't throw given the above
        def = new T(ConvertToCpp<T>(tup[1]));
    }

    // Already checked
    /*
    PythonType ptype_type = DetermineType(tup[0]);
    if(ptype_type != PythonType::String)
        throw OptionException("\"Type\" element of tuple is not a bool", key, "type", GetPyClass(tup[0])); 
    */

    PythonType ptype_required = DetermineType(tup[2]);
    if(ptype_required != PythonType::Bool)
        throw OptionException("\"Required\" element of tuple is not a bool", key, "type", GetPyClass(tup[2])); 

    PythonType ptype_help = DetermineType(tup[4]);
    if(ptype_help != PythonType::String)
        throw OptionException("\"Help\" element of tuple is not a string", key, "type", GetPyClass(tup[4])); 


    bool req = boost::python::extract<bool>(tup[2]);
    std::string help = boost::python::extract<std::string>(tup[4]);
    python_helper::PythonType pytype = StrToPythonType(boost::python::extract<std::string>(tup[0]));


    //! \todo Check to make sure validator object is callable
    
    // Check if validator is given. If not, use EmptyValidator
    typename OptionHolder<T>::ValidatorFunc validator = EmptyValidator<T>;

    if(DetermineType(tup[3]) != PythonType::None)
        validator = std::bind(ValidateWrapper<T>, tup[3], std::placeholders::_1);

    

    return OptionBasePtr(new OptionHolder<T>(key, def, validator, req, pytype, help)); 
}



OptionBasePtr OptionHolderFactory(const std::string & key, const boost::python::object & obj)
{
    PythonType ptype = DetermineType(obj);
    if(ptype != PythonType::Tuple)
        throw OptionException("Object for option is not a tuple", key, "pythontype", GetPyClass(obj)); 


    boost::python::tuple tup = boost::python::extract<boost::python::tuple>(obj);


    int length = boost::python::extract<int>(tup.attr("__len__")());
    if(length != 5)
        throw OptionException("Python options tuple does not have 5 elements", key, "length", std::to_string(length)); 


    if(!TestConvertToCpp<std::string>(tup[0]))
        throw OptionException("\"Type\" element of tuple is not a string", key, "type", GetPyClass(tup[0]));


    std::string type = ConvertToCpp<std::string>(tup[0]);


    switch(StrToPythonType(type))
    {
        case PythonType::Bool:
            return CreateOptionHolder<bool>(key, tup); 
        case PythonType::Int:
            return CreateOptionHolder<int>(key, tup); 
        case PythonType::Float:
            return CreateOptionHolder<double>(key, tup); 
        case PythonType::String:
            return CreateOptionHolder<std::string>(key, tup); 
        case PythonType::ListBool:
            return CreateOptionHolder<std::vector<bool>>(key, tup); 
        case PythonType::ListInt:
            return CreateOptionHolder<std::vector<int>>(key, tup); 
        case PythonType::ListFloat:
            return CreateOptionHolder<std::vector<double>>(key, tup); 
        case PythonType::ListString:
            return CreateOptionHolder<std::vector<std::string>>(key, tup); 
        default:
            throw OptionException("Cannot convert python type to option", key, "type", type); 
    }
}




} // close namespace detail
} // close namespace datastore
} // close namespace bpmodule

