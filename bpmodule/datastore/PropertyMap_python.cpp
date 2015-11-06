/*! \file
 *
 * \brief Storage of generic data via (key, value) pair (source for python functions)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/datastore/PropertyMap.hpp"
#include "bpmodule/python_helper/Convert.hpp"
#include "bpmodule/tensor/Tensor.hpp"


//! \todo Add tensor stuff?

using bpmodule::python_helper::ConvertToCpp;
using bpmodule::python_helper::ConvertToPy;
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

        // make sure it's a tuple
        std::string cl = boost::python::extract<std::string>(olist[i].attr("__class__").attr("__name__"));

        if(cl != "tuple")
        {
            throw GeneralException("PropertyMap element is not a tuple",
                                   "location", "PropertyMap",
                                   "mapkey", key,
                                   "element", i,
                                   "fromtype", cl,
                                   "totype", "tuple");
        }


        // check the number of elements in the tuple
        int tuplelen = boost::python::extract<int>(olist[i].attr("__len__")());
        if(tuplelen != 2)
        {
            throw GeneralException("PropertyMap element is not a tuple of 2 elements",
                                   "location", "PropertyMap",
                                   "mapkey", key,
                                   "element", i,
                                   "length", tuplelen);
        }


        // actually convert now
        // may throw
        key = ConvertToCpp<std::string>(olist[i][0]);

        if(opmap_.count(key))
            throw GeneralException("Duplicate key found on construction",
                                   "location", "PropertyMap",
                                   "mapkey", key);


        boost::python::object dat = olist[i][1];
        Set_(key, std::move(detail::GenericHolderFactory(dat)));
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
    // catch errors from conversions in GenericHolderFactory
    try {
        Set_(key, detail::GenericHolderFactory(value));
    }
    catch(GeneralException & ex)
    {
        // append key info if GenericHolderFactory throws
        ex.AppendInfo("mapkey", key);
        throw;
    }
}



} // close namespace datastore
} // close namespace bpmodule

