/*! \file
 *
 * \brief Storage of generic data via (key, value) pair (source for python functions)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/datastore/PropertyMap.hpp"
#include "bpmodule/python_helper/Convert.hpp"
#include "bpmodule/tensor/Tensor.hpp"

// included via Python_stdconvert.hpp
//#include <boost/python.hpp>


using bpmodule::python_helper::ConvertListToVec;


namespace bpmodule {
namespace datastore {

// construct from python list
PropertyMap::PropertyMap(const boost::python::list & olist)
{
    int optlen = boost::python::extract<int>(olist.attr("__len__")());

    for(int i = 0; i < optlen; i++)
    {
        std::string key = boost::python::extract<std::string>(olist[i][0]);
        Set_(key, PropertyMap::PropPlaceholder_(olist[i][1]));
    }
}




template<>
boost::python::object PropertyMap::GetCopy<>(const std::string & key) const
{
    // some of the dirty details handled by to_python converters
    // see Python_stdconvert.hpp

    std::string type = GetType(key);

    if(type == typeid(bool).name())
        return boost::python::object(GetRef<bool>(key));

    else if(type == typeid(long).name())
        return boost::python::object(GetRef<long>(key));

    else if(type == typeid(double).name())
        return boost::python::object(GetRef<double>(key));

    else if(type == typeid(std::string).name())
        return boost::python::object(GetRef<std::string>(key));

    else if(type == typeid(std::vector<long>).name())
        return boost::python::object(GetRef<std::vector<long>>(key));

    else if(type == typeid(std::vector<double>).name())
        return boost::python::object(GetRef<std::vector<double>>(key));

    else if(type == typeid(std::vector<std::string>).name())
        return boost::python::object(GetRef<std::vector<std::string>>(key));

    else if(type == typeid(tensor::Tensor).name())
        return boost::python::object(GetRef<tensor::Tensor>(key));

    else
        throw exception::GeneralException("Unable to convert C++ type to python",
                                 {
                                    { "Location", "PropertyMap" },
                                    { "Key", key },
                                    { "Type", type }
                                 }
                               );
}


template<>
void PropertyMap::Set(const std::string & key, const boost::python::object & value)
{
    // catch errors from conversions in PropPlaceholder_
    try {
        Set_(key, PropertyMap::PropPlaceholder_(value));
    }
    catch(exception::GeneralException & bpe)
    {
        // append key info
        bpe.AppendInfo({
                         {"Key", key}
                       });
        throw bpe;
    }
}



PropertyMap::PropPlaceholderPtr PropertyMap::PropertyMap::PropPlaceholder_(const boost::python::object & value)
{
    std::string cl = boost::python::extract<std::string>(value.attr("__class__").attr("__name__"));
    if(cl == "bool")
        return PropertyMap::PropPlaceholderPtr(new PropHolder<bool>(boost::python::extract<bool>(value)));
    else if(cl == "int")
        return PropertyMap::PropPlaceholderPtr(new PropHolder<long>(boost::python::extract<long>(value)));
    else if(cl == "float")
        return PropertyMap::PropPlaceholderPtr(new PropHolder<double>(boost::python::extract<double>(value)));
    else if(cl == "str")
        return PropertyMap::PropPlaceholderPtr(new PropHolder<std::string>(boost::python::extract<std::string>(value)));
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
                throw exception::GeneralException("Passed a heterogeneous container",
                                        {
                                          { "First type", cl2 },
                                          { "This type", cltmp }
                                        }
                                       );
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
            throw exception::GeneralException("Unable to convert python type to C++ (for list)",
                                     {
                                        { "Location", "PropertyMap" },
                                        { "Type", cl2 }
                                     }
                                   );
        }
    }
    else
    {
        throw exception::GeneralException("Unable to convert python type to C++",
                                 {
                                    { "Location", "PropertyMap" },
                                    { "Type", cl }
                                 }
                               );
    }
}


} // close namespace datastore
} // close namespace bpmodule

