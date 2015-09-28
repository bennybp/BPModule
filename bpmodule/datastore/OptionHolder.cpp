/*! \file
 *
 * \brief Storage of generic data via (key, value) pair (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/datastore/OptionHolder.hpp"
#include "bpmodule/python_helper/Convert.hpp"


using bpmodule::python_helper::PythonType;
using bpmodule::python_helper::ConvertToCpp;
using bpmodule::python_helper::TestConvertToCpp;
using bpmodule::python_helper::DetermineType;
using bpmodule::python_helper::GetPyClass;
using bpmodule::python_helper::StrToPythonType;
using bpmodule::exception::OptionException;


namespace bpmodule {
namespace datastore {
namespace detail {

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


    PythonType ptype_required = DetermineType(tup[2]);
    if(ptype_required != PythonType::Bool)
        throw OptionException("\"Required\" element of tuple is not a bool", key, "type", PythonTypeToStr(ptype_required)); 

    bool req = boost::python::extract<bool>(tup[2]);

    //! \todo Check to make sure validator object is callable
    
    // Check if validator is given. If not, use EmptyValidator
    typename OptionHolder<T>::ValidatorFunc validator = EmptyValidator<T>;

    if(DetermineType(tup[3]) != PythonType::None)
        validator = std::bind(ValidateWrapper<T>, tup[3], std::placeholders::_1);

    return OptionBasePtr(new OptionHolder<T>(key, def, validator, req)); 
}



OptionBasePtr OptionHolderFactory(const std::string & key, const boost::python::object & obj)
{
    PythonType ptype = DetermineType(obj);
    if(ptype != PythonType::Tuple)
        throw OptionException("Object for option is not a tuple", key, "pythontype", PythonTypeToStr(ptype)); 


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

