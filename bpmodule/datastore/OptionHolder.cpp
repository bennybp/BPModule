/*! \file
 *
 * \brief Storage of generic data via (key, value) pair (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/datastore/OptionHolder.hpp"
#include "bpmodule/python_helper/Convert.hpp"


using bpmodule::python_helper::PythonType;
using bpmodule::python_helper::ConvertToCpp;
using bpmodule::python_helper::ConvertToPy;
using bpmodule::python_helper::DetermineType;



namespace bpmodule {
namespace datastore {
namespace detail {

template<typename T>
bool ValidateWrapper(const boost::python::object & val, T arg)
{
    return val(arg);
}


template<typename T>
bool EmptyValidator(T arg)
{
    return true;
}


template<typename T>
OptionBasePtr CreateOptionHolder(const boost::python::tuple & tup)
{
    PythonType ptype_default = DetermineType(tup[1]);

    // value is empty at this point
    T * val = nullptr;
    T * def = nullptr;

    if(ptype_default != PythonType::None)
        def = new T(ConvertToCpp<T>(tup[1]));

    bool req = boost::python::extract<bool>(tup[2]);

    //! \todo Check to make sure object is callable
    //
    // Check if validator is given. If not, use EmptyValidator
    typename OptionHolder<T>::ValidatorFunc validator = EmptyValidator<T>;

    if(DetermineType(tup[3]) != PythonType::None)
        validator = std::bind(ValidateWrapper<T>, tup[3], std::placeholders::_1);

    //! \todo expert option
    return OptionBasePtr(new OptionHolder<T>(val, def, validator, req, false)); 
}



OptionBasePtr OptionHolderFactory(const boost::python::object & obj)
{
    PythonType ptype = DetermineType(obj);
    if(ptype != PythonType::Tuple)
        throw exception::OptionException("Object for options is not a tuple", PythonTypeToStr(ptype)); 

    boost::python::tuple tup = boost::python::extract<boost::python::tuple>(obj);

    int length = boost::python::extract<int>(tup.attr("__len__")());
    if(length != 5)
        throw exception::OptionException("Tuple does not have 5 elements", "tuple"); 

    // type, default, required, validator, help
    // (help is not parsed at the moment)
    PythonType ptype_type     = DetermineType(tup[0]);
    PythonType ptype_required = DetermineType(tup[2]);

    if(ptype_type != PythonType::String)
        throw exception::OptionException("\"Type\" element of tuple is not a string", PythonTypeToStr(ptype_type)); 

    if(ptype_required != PythonType::Bool)
        throw exception::OptionException("\"Required\" element of tuple is not a bool", PythonTypeToStr(ptype_required)); 


    std::string type = boost::python::extract<std::string>(tup[0]);


    switch(python_helper::StrToPythonType(type))
    {
        case PythonType::Bool:
            return CreateOptionHolder<bool>(tup); 
        case PythonType::Int:
            return CreateOptionHolder<int>(tup); 
        case PythonType::Float:
            return CreateOptionHolder<double>(tup); 
        case PythonType::String:
            return CreateOptionHolder<std::string>(tup); 
        case PythonType::ListBool:
            return CreateOptionHolder<std::vector<bool>>(tup); 
        case PythonType::ListInt:
            return CreateOptionHolder<std::vector<int>>(tup); 
        case PythonType::ListFloat:
            return CreateOptionHolder<std::vector<double>>(tup); 
        case PythonType::ListString:
            return CreateOptionHolder<std::vector<std::string>>(tup); 
        default:
            throw exception::OptionException("Cannot convert python type to option", PythonTypeToStr(ptype_type)); 
    }
}




} // close namespace detail
} // close namespace datastore
} // close namespace bpmodule

