/*! \file
 *
 * \brief Storage of options (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/datastore/OptionMap.hpp"
#include "bpmodule/python_helper/Convert.hpp"


//! \todo Add tensor stuff?

using bpmodule::python_helper::ConvertToCpp;
using bpmodule::python_helper::ConvertToPy;
using bpmodule::exception::PythonConvertException;


namespace bpmodule {
namespace datastore {

// construct from python dict
OptionMap::OptionMap(const boost::python::dict & opt)
{
    boost::python::list keys = opt.keys();
    int keylen = boost::python::extract<int>(keys.attr("__len__")());

    try {
        for(int i = 0; i < keylen; i++)
        {
            std::string key = ConvertToCpp<std::string>(keys[i]);

            if(opmap_.count(key))
                throw exception::MapException("Duplicate key on construction", "OptionMap", key); 

            // this will throw needed exceptions
            opmap_.emplace(key, detail::OptionHolderFactory(opt[key]));
        }
    }
    catch(PythonConvertException & ex) 
    {
        // should always be a PythonConvertException?
        // Don't catch the MapException, let that go through
        ex.AppendInfo({ {"location", "OptionMap"} });
        throw;
    }
}

void OptionMap::ChangePy(const std::string & key, const boost::python::object & obj)
{
    GetOrThrow_(key)->ChangeValue(obj);
}

void OptionMap::Merge(const boost::python::dict & opt)
{
    boost::python::list keys = opt.keys();
    int keylen = boost::python::extract<int>(keys.attr("__len__")());

    // store in temporary map
    std::map<std::string, detail::OptionBasePtr> tmpmap;
 
    try {
        for(int i = 0; i < keylen; i++)
        {
            std::string key = ConvertToCpp<std::string>(keys[i]);

            if(!opmap_.count(key))
                throw exception::MapException("Key not found form merging", "OptionMap", key); 

            // this will throw needed exceptions
            tmpmap.emplace(key, detail::OptionHolderFactory(opt[key]));
        }
    }
    catch(PythonConvertException & ex) 
    {
        // should always be a PythonConvertException?
        // Don't catch the MapException, let that go through
        ex.AppendInfo({ {"location", "OptionMap"} });
        throw;
    }

    // now merge
    for(const auto & it : tmpmap)
        opmap_.at(it.first) = it.second;
}


} // close namespace datastore
} // close namespace bpmodule

