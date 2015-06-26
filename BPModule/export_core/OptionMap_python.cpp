#include "BPModule/core/Output.h"
#include "BPModule/export_core/OptionMap_python.h"
#include "BPModule/export_core/Python_stdconvert.h"

#include <boost/python.hpp>

namespace out = bpmodule::output;

namespace bpmodule {
namespace export_python {


// Some wrappers for OptionMap Get/Set member
void OptionMap_Set_Helper(OptionMap * op, const std::string & key,
                          const boost::python::object & value,
                          const std::string & help)
{
    std::string cl = boost::python::extract<std::string>(value.attr("__class__").attr("__name__"));

    if(cl == "int")
        op->Set(key, static_cast<long>(boost::python::extract<long>(value)), help);
    else if(cl == "float")
        op->Set(key, static_cast<double>(boost::python::extract<double>(value)), help);
    else if(cl == "str")
        op->Set(key, static_cast<std::string>(boost::python::extract<std::string>(value)), help);
    else if(cl == "list")
    {
        // get type of first element
        boost::python::list lst = boost::python::extract<boost::python::list>(value);
        int length = boost::python::extract<int>(lst.attr("__len__")());
        if(length == 0)
        {
            // plain string vector if empty
            std::vector<std::string> v;
            op->Set(key, std::vector<std::string>(), help);
        }
        else
        {
            std::string cl2 = boost::python::extract<std::string>(lst[0].attr("__class__").attr("__name__"));
            if(cl2 == "int")
                op->Set(key, ConvertListToVec<long>(lst), help);
            else if(cl2 == "float")
                op->Set(key, ConvertListToVec<double>(lst), help);
            else if(cl2 == "str")
                op->Set(key, ConvertListToVec<std::string>(lst), help);
            else
                out::Error("Unknown type %1% in list for key %2%\n", cl, key);
        }
    }
    else
        out::Error("Unknown type %1% for key %2%\n", cl, key);
}



boost::python::object OptionMap_Get_Helper(const OptionMap * op, const std::string & key)
{
    std::string type = op->GetType(key);

    if(type == typeid(long).name())
        return boost::python::object(op->Get<long>(key));
    else if(type == typeid(double).name())
        return boost::python::object(op->Get<double>(key));
    else if(type == typeid(std::string).name())
        return boost::python::object(op->Get<std::string>(key));
    else if(type == typeid(std::vector<long>).name())
        return boost::python::object(op->Get<std::vector<long>>(key));
    else if(type == typeid(std::vector<double>).name())
        return boost::python::object(op->Get<std::vector<double>>(key));
    else if(type == typeid(std::vector<std::string>).name())
        return boost::python::object(op->Get<std::vector<std::string>>(key));
    else
    {
        out::Error("Unable to deduce type: %1%\n", type);
        return boost::python::object();
    }
}



// Convert lists <-> OptionMaps
OptionMap ListToOptionMap(const boost::python::list & olist)
{
    OptionMap op;
    int optlen = boost::python::extract<int>(olist.attr("__len__")());

    for(int i = 0; i < optlen; i++)
    {
        std::string key = boost::python::extract<std::string>(olist[i][0]);
        std::string help = boost::python::extract<std::string>(olist[i][2]);
        OptionMap_Set_Helper(&op, key, olist[i][1], help);
    }
    return op;
}



boost::python::list OptionMapToList(const OptionMap & op)
{
    boost::python::list lst;
    auto keys = op.GetKeys();

    for(auto & it : keys)
        lst.append(boost::python::make_tuple(it, OptionMap_Get_Helper(&op, it), op.GetHelp(it)));

    return lst;
}


} // close namespace export_python
} // close namespace bpmodule

