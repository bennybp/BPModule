#include "BPModule/core/Output.hpp"
#include "BPModule/export_core/OptionMap_python.hpp"
#include "BPModule/export_core/Python_stdconvert.hpp"

#include <boost/python.hpp>

namespace out = bpmodule::output;
namespace bpy = boost::python;



namespace bpmodule {
namespace export_python {


std::unique_ptr<OptionPlaceholder> OptionPlaceholder_FromPython(const bpy::object & value)
{
    std::string cl = bpy::extract<std::string>(value.attr("__class__").attr("__name__"));

    if(cl == "int")
        return std::unique_ptr<OptionPlaceholder>(new OptionHolder<long>(bpy::extract<long>(value)));
    else if(cl == "float")
        return std::unique_ptr<OptionPlaceholder>(new OptionHolder<double>(bpy::extract<double>(value)));
    else if(cl == "str")
        return std::unique_ptr<OptionPlaceholder>(new OptionHolder<std::string>(bpy::extract<std::string>(value)));
    else if(cl == "list")
    {
        // get type of first element
        bpy::list lst = bpy::extract<bpy::list>(value);
        int length = bpy::extract<int>(lst.attr("__len__")());
        if(length == 0)
        {
            out::Error("Empty list: %\n", cl);
            return std::unique_ptr<OptionPlaceholder>(new OptionHolder<std::string>("????"));
        }
        else
        {
            // todo - check all
            // check the first elements or find a heterogeneous container
            std::string cl2 = bpy::extract<std::string>(lst[0].attr("__class__").attr("__name__"));
            if(cl2 == "int")
                return std::unique_ptr<OptionPlaceholder>(new OptionHolder<std::vector<long>>(ConvertListToVec<long>(lst)));
            else if(cl2 == "float")
                return std::unique_ptr<OptionPlaceholder>(new OptionHolder<std::vector<double>>(ConvertListToVec<double>(lst)));
            else if(cl2 == "str")
                return std::unique_ptr<OptionPlaceholder>(new OptionHolder<std::vector<std::string>>(ConvertListToVec<std::string>(lst)));
            else
            {
                out::Error("Unknown type %1% in list\n", cl);
                return std::unique_ptr<OptionPlaceholder>(new OptionHolder<std::string>("????"));
            }
        }
    }
    else
    {
        out::Error("Unknown type %1%\n", cl);
        return std::unique_ptr<OptionPlaceholder>(new OptionHolder<std::string>("????"));
    }
}


void OptionMap_Change_Helper(OptionMap * op, const std::string & key, const bpy::object & value)
{
    op->Change(key, OptionPlaceholder_FromPython(value));
}


void OptionMap_InitDefault_Helper(OptionMap * op, const std::string & key, const bpy::object & def, const std::string & help)
{
    op->InitDefault(key, OptionPlaceholder_FromPython(def), help);
}



bpy::object OptionMap_Get_Helper(const OptionMap * op, const std::string & key)
{
    std::string type = op->GetType(key);

    if(type == typeid(long).name())
        return bpy::object(op->Get<long>(key));
    else if(type == typeid(double).name())
        return bpy::object(op->Get<double>(key));
    else if(type == typeid(std::string).name())
        return bpy::object(op->Get<std::string>(key));
    else if(type == typeid(std::vector<long>).name())
        return bpy::object(op->Get<std::vector<long>>(key));
    else if(type == typeid(std::vector<double>).name())
        return bpy::object(op->Get<std::vector<double>>(key));
    else if(type == typeid(std::vector<std::string>).name())
        return bpy::object(op->Get<std::vector<std::string>>(key));
    else
    {
        out::Error("Unable to deduce type: %1%\n", type);
        return bpy::object();
    }
}



OptionMap OptionMap_InitFromList_Helper(const bpy::list & olist)
{
    OptionMap op;
    int optlen = bpy::extract<int>(olist.attr("__len__")());

    for(int i = 0; i < optlen; i++)
    {
        std::string key = bpy::extract<std::string>(olist[i][0]);
        std::string help = bpy::extract<std::string>(olist[i][2]);
        OptionMap_InitDefault_Helper(&op, key, olist[i][1], help);
    }

    return op;
}



} // close namespace export_python
} // close namespace bpmodule

