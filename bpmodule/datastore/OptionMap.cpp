/*! \file
 *
 * \brief Storage of options (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include <boost/python.hpp>

#include "bpmodule/datastore/OptionMap.hpp"
#include "bpmodule/python_helper/Convert.hpp"
#include "bpmodule/output/Output.hpp"


//! \todo Add tensor stuff?

using bpmodule::python_helper::ConvertToCpp;
using bpmodule::python_helper::ConvertToPy;
using bpmodule::python_helper::TestConvertToCpp;
using bpmodule::python_helper::TestConvertToPy;
using bpmodule::exception::OptionException;


namespace bpmodule {
namespace datastore {

// construct from python dict
OptionMap::OptionMap(const boost::python::dict & opt)
{
    boost::python::list keys = opt.keys();

    // shouldn't throw, should it?
    int keylen = boost::python::extract<int>(keys.attr("__len__")());

    for(int i = 0; i < keylen; i++)
    {
        if(!TestConvertToCpp<std::string>(keys[i]))
            throw OptionException("Key in OptionMap dictionary is not a string", "(unknown)",
                                  "element", std::to_string(i)); 
        
        std::string key = ConvertToCpp<std::string>(keys[i]);

        if(opmap_.count(key))
            throw OptionException("Duplicate key on construction",
                                   "element", std::to_string(i)); 

        // this will throw needed exceptions
        opmap_.emplace(key, detail::OptionHolderFactory(key, opt[key]));
    }
}


void OptionMap::ChangePy(const std::string & key, const boost::python::object & obj)
{
    detail::OptionBasePtr & ptr = GetOrThrow_(key);

    ptr->ChangePy(obj);
}


void OptionMap::ChangePyDict(const boost::python::dict & opt)
{
    using std::swap;

    boost::python::list keys = opt.keys();
    int keylen = boost::python::extract<int>(keys.attr("__len__")());

    // for strong exception guarantee:
    // copy the current object, modify that, then swap

    OptionMap tmp(*this);

    for(int i = 0; i < keylen; i++)
    {
        if(!TestConvertToCpp<std::string>(keys[i]))
            throw OptionException("Cannot convert python dictionary index to string", "(unknown)", "element", std::to_string(i));

        std::string key = ConvertToCpp<std::string>(keys[i]);

        if(!Has(key))
            throw OptionException("Python dictionary has a key that I do not", key, "element", std::to_string(i));

        tmp.ChangePy(key, opt[key]); 
    }

    swap(*this, tmp);
}


} // close namespace datastore
} // close namespace bpmodule

