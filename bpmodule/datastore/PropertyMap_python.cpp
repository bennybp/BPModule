/*! \file
 *
 * \brief Storage of generic data via (key, value) pair (source for python functions)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/datastore/PropertyMap.hpp"
#include "bpmodule/python_helper/Convert.hpp"
#include "bpmodule/tensor/Tensor.hpp"
#include "bpmodule/exception/PythonConvertException.hpp"


//! \todo Add tensor stuff?

using bpmodule::python_helper::ConvertToCpp;
using bpmodule::python_helper::ConvertToPy;
using bpmodule::exception::PythonConvertException;


namespace bpmodule {
namespace datastore {

// construct from python list
PropertyMap::PropertyMap(const boost::python::list & olist)
{
    int optlen = boost::python::extract<int>(olist.attr("__len__")());

    for(int i = 0; i < optlen; i++)
    {
        std::string key("(unknown)");

        // make sure it's a tuple
        std::string cl = boost::python::extract<std::string>(olist[i].attr("__class__").attr("__name__"));

        if(cl != "tuple")
        {
            PythonConvertException ex("PropertyMap element is not a tuple", "PropertyMap", key, cl, "tuple");
            ex.AppendInfo({ {"element", std::to_string(i) } });
            throw ex;
        }


        // check the number of elements in the tuple
        int tuplelen = boost::python::extract<int>(olist[i].attr("__len__")());
        if(tuplelen != 2)
        {
            PythonConvertException ex("PropertyMap element is not a tuple of 2 elements", "PropertyMap", key, cl, "tuple");
            ex.AppendInfo({ {"element", std::to_string(i) }, {"length", std::to_string(tuplelen)} });
            throw ex;
        }


        // actually convert now
        try {
            key = ConvertToCpp<std::string>(olist[i][0]);

            if(opmap_.count(key))
                throw exception::MapException("Duplicate key found on construction", "PropertyMap", key); 


            boost::python::object dat = olist[i][1];
            Set_(key, std::move(detail::PropHolderFactory(dat)));
        }
        catch(PythonConvertException & ex) 
        {
            // should always be a PythonConvertException?
            // Don't catch the MapException, let that go through
            ex.AppendInfo({ {"location", "PropertyMap"}, {"key", key} });
            throw;
        }
    }
}




template<>
boost::python::object PropertyMap::GetCopy<>(const std::string & key) const
{
    // may throw
    return GetOrThrow_(key).value->GetPy(); 
}


template<>
void PropertyMap::Set<>(const std::string & key, const boost::python::object & value)
{
    // catch errors from conversions in PropHolderFactory
    try {
        Set_(key, detail::PropHolderFactory(value));
    }
    catch(PythonConvertException & ex)
    {
        // append key info
        ex.AppendInfo({ {"key", key} });
        throw;
    }
}



} // close namespace datastore
} // close namespace bpmodule

