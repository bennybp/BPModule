#include "BPModule/core/Output.hpp"
#include "BPModule/core/PropertyMap.hpp"
#include "BPModule/export_core/Python_stdconvert.hpp"

#include <boost/python.hpp>

namespace out = bpmodule::output;
namespace bpy = boost::python;

using bpmodule::export_python::ConvertListToVec;


namespace bpmodule {

// construct from python list
PropertyMap::PropertyMap(const bpy::list & olist)
{
    int optlen = bpy::extract<int>(olist.attr("__len__")());

    for(int i = 0; i < optlen; i++)
    {
        std::string key = bpy::extract<std::string>(olist[i][0]);
        Add_(key, PropertyMap::PropPlaceholder_(olist[i][1]));
    }
}


template<>
bpy::object PropertyMap::GetCopy<>(const std::string & key) const
{
    // some of the dirty details handled by to_python converters
    // see Python_stdconvert.hpp

    std::string type = GetType(key);

    if(type == typeid(bool).name())
        return bpy::object(GetRef<bool>(key));

    else if(type == typeid(long).name())
        return bpy::object(GetRef<long>(key));

    else if(type == typeid(double).name())
        return bpy::object(GetRef<double>(key));

    else if(type == typeid(std::string).name())
        return bpy::object(GetRef<std::string>(key));

    else if(type == typeid(std::vector<long>).name())
        return bpy::object(GetRef<std::vector<long>>(key));

    else if(type == typeid(std::vector<double>).name())
        return bpy::object(GetRef<std::vector<double>>(key));

    else if(type == typeid(std::vector<std::string>).name())
        return bpy::object(GetRef<std::vector<std::string>>(key));

    else
        throw BPModuleException("Unable to convert C++ type to python",
                                 {
                                    { "Location", "PropertyMap" },
                                    { "Key", key },
                                    { "Type", type }
                                 }
                               );
}


template<>
void PropertyMap::Add(const std::string & key, const bpy::object & value)
{
    try {
        Add_(key, PropertyMap::PropPlaceholder_(value));
    }
    catch(BPModuleException & bpe)
    {
        // append key info
        bpe.AppendInfo({
                         {"Key", key}
                       });
        throw bpe;
    }
}


template<>
void PropertyMap::Change(const std::string & key, const bpy::object & value)
{
    try {
        Change_(key, PropertyMap::PropPlaceholder_(value));
    }
    catch(BPModuleException & bpe)
    {
        // append key info
        bpe.AppendInfo({
                         {"Key", key}
                       });
        throw bpe;
    }
}



std::unique_ptr<PropertyMap::PropPlaceholder> PropertyMap::PropertyMap::PropPlaceholder_(const bpy::object & value)
{
    std::string cl = bpy::extract<std::string>(value.attr("__class__").attr("__name__"));
    if(cl == "bool")
        return std::unique_ptr<PropertyMap::PropPlaceholder>(new PropHolder<bool>(bpy::extract<bool>(value)));
    else if(cl == "int")
        return std::unique_ptr<PropertyMap::PropPlaceholder>(new PropHolder<long>(bpy::extract<long>(value)));
    else if(cl == "float")
        return std::unique_ptr<PropertyMap::PropPlaceholder>(new PropHolder<double>(bpy::extract<double>(value)));
    else if(cl == "str")
        return std::unique_ptr<PropertyMap::PropPlaceholder>(new PropHolder<std::string>(bpy::extract<std::string>(value)));
    else if(cl == "list")
    {
        // get type of first element
        bpy::list lst = bpy::extract<bpy::list>(value);
        int length = bpy::extract<int>(lst.attr("__len__")());
        if(length == 0)
            throw BPModuleException("Empty list passed from python");

        std::string cl2 = bpy::extract<std::string>(lst[0].attr("__class__").attr("__name__"));

        // check if this is a homogeneous container
        for(int i = 1; i < length; i++)
        {
            std::string cltmp = bpy::extract<std::string>(lst[i].attr("__class__").attr("__name__"));
            if(cl2 != cltmp)
                throw BPModuleException("Passed a heterogeneous container",
                                        {
                                          { "First type", cl2 },
                                          { "This type", cltmp }
                                        }
                                       );
        }

        // now parse list
        if(cl2 == "bool")
            return std::unique_ptr<PropertyMap::PropPlaceholder>(new PropHolder<std::vector<bool>>(ConvertListToVec<bool>(lst)));
        if(cl2 == "int")
            return std::unique_ptr<PropertyMap::PropPlaceholder>(new PropHolder<std::vector<long>>(ConvertListToVec<long>(lst)));
        else if(cl2 == "float")
            return std::unique_ptr<PropertyMap::PropPlaceholder>(new PropHolder<std::vector<double>>(ConvertListToVec<double>(lst)));
        else if(cl2 == "str")
            return std::unique_ptr<PropertyMap::PropPlaceholder>(new PropHolder<std::vector<std::string>>(ConvertListToVec<std::string>(lst)));
        else
        {
            throw BPModuleException("Unable to convert python type to C++ (for list)",
                                     {
                                        { "Location", "PropertyMap" },
                                        { "Type", cl2 }
                                     }
                                   );
        }
    }
    else
    {
        throw BPModuleException("Unable to convert python type to C++",
                                 {
                                    { "Location", "PropertyMap" },
                                    { "Type", cl }
                                 }
                               );
    }
}


} // close namespace bpmodule

