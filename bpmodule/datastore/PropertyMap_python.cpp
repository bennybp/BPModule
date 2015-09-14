/*! \file
 *
 * \brief Storage of generic data via (key, value) pair (source for python functions)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/datastore/PropertyMap.hpp"
#include "bpmodule/python_helper/Convert.hpp"
#include "bpmodule/tensor/Tensor.hpp"
#include "bpmodule/exception/PythonConvertException.hpp"


using bpmodule::python_helper::ConvertListToVec;
using bpmodule::python_helper::ConvertVecToList;
using bpmodule::python_helper::ConvertToPy;
using bpmodule::python_helper::ConvertToCpp;
using bpmodule::exception::PythonConvertException;
using bpmodule::exception::GeneralException;


namespace bpmodule {
namespace datastore {

// construct from python list
PropertyMap::PropertyMap(const boost::python::list & olist)
{
    int optlen = boost::python::extract<int>(olist.attr("__len__")());


    for(int i = 0; i < optlen; i++)
    {
        std::string key("(unknown)");

        try {
            key = ConvertToCpp<std::string>(olist[i][0]);
            Set_(key, PropPlaceholder_(olist[i][1]));
        }
        catch(GeneralException & ex)
        {
            ex.AppendInfo({ {"location", "PropertyMap"}, {"key", key} });
        }
    }
}




template<>
boost::python::object PropertyMap::GetCopy<>(const std::string & key) const
{
    std::string type = GetType(key);

    if(type == typeid(bool).name())
        return ConvertToPy(GetRef<bool>(key));

    else if(type == typeid(long).name())
        return ConvertToPy(GetRef<long>(key));

    else if(type == typeid(double).name())
        return ConvertToPy(GetRef<double>(key));

    else if(type == typeid(std::string).name())
        return ConvertToPy(GetRef<std::string>(key));

    else if(type == typeid(std::vector<long>).name())
        return ConvertVecToList(GetRef<std::vector<long>>(key));

    else if(type == typeid(std::vector<double>).name())
        return ConvertVecToList(GetRef<std::vector<double>>(key));

    else if(type == typeid(std::vector<std::string>).name())
        return ConvertVecToList(GetRef<std::vector<std::string>>(key));

    else if(type == typeid(tensor::Tensor).name())
        return ConvertToPy(GetRef<tensor::Tensor>(key));

    else
        throw exception::PythonConvertException("Invalid type to convert to python",
                                                "PropertyMap", key, type, "boost::python::object"); 
}


template<>
void PropertyMap::Set(const std::string & key, const boost::python::object & value)
{
    // catch errors from conversions in PropPlaceholder_
    try {
        Set_(key, PropPlaceholder_(value));
    }
    catch(exception::GeneralException & ex)
    {
        // append key info
        ex.AppendInfo({ {"key", key} });
        throw ex;
    }
}



PropertyMap::PropPlaceholderPtr PropertyMap::PropertyMap::PropPlaceholder_(const boost::python::object & value)
{
    std::string cl = boost::python::extract<std::string>(value.attr("__class__").attr("__name__"));
    if(cl == "bool")
        return PropertyMap::PropPlaceholderPtr(new PropHolder<bool>(ConvertToCpp<bool>(value)));
    else if(cl == "int")
        return PropertyMap::PropPlaceholderPtr(new PropHolder<long>(ConvertToCpp<long>(value)));
    else if(cl == "float")
        return PropertyMap::PropPlaceholderPtr(new PropHolder<double>(ConvertToCpp<double>(value)));
    else if(cl == "str")
        return PropertyMap::PropPlaceholderPtr(new PropHolder<std::string>(ConvertToCpp<std::string>(value)));
    else if(cl == "list")
    {
        // get type of first element
        boost::python::list lst = boost::python::extract<boost::python::list>(value);
        int length = boost::python::extract<int>(lst.attr("__len__")());
        if(length == 0)
            throw exception::GeneralException("Empty list passed from python");

        std::string cl2 = boost::python::extract<std::string>(lst[0].attr("__class__").attr("__name__"));

        // check if this is a homogeneous container
        for(int i = 1; i < length; i++)
        {
            std::string cltmp = boost::python::extract<std::string>(lst[i].attr("__class__").attr("__name__"));
            if(cl2 != cltmp)
                throw exception::PythonConvertException("Cannot convert heterogeneous container", cltmp, cl2);
        }

        // now parse list
        if(cl2 == "bool")
            return PropertyMap::PropPlaceholderPtr(new PropHolder<std::vector<bool>>(ConvertListToVec<bool>(lst)));
        if(cl2 == "int")
            return PropertyMap::PropPlaceholderPtr(new PropHolder<std::vector<long>>(ConvertListToVec<long>(lst)));
        else if(cl2 == "float")
            return PropertyMap::PropPlaceholderPtr(new PropHolder<std::vector<double>>(ConvertListToVec<double>(lst)));
        else if(cl2 == "str")
            return PropertyMap::PropPlaceholderPtr(new PropHolder<std::vector<std::string>>(ConvertListToVec<std::string>(lst)));
        else
        {
        throw exception::PythonConvertException("Invalid type to convert from python",
                                                "PropertyMap", "(see below)", cl2, "boost::python::object", "In converting a list"); 
        }
    }
    else
    {
        throw exception::PythonConvertException("Invalid type to convert from python",
                                                "PropertyMap", "(see below)", cl, "boost::python::object"); 
    }
}


} // close namespace datastore
} // close namespace bpmodule

