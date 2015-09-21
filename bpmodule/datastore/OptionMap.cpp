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
using bpmodule::exception::PythonConvertException;
using bpmodule::exception::GeneralException;


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
            try {
                opmap_.emplace(key, detail::OptionHolderFactory(opt[key]));
            }
            catch(GeneralException & ex)
            {
                ex.AppendInfo( { { "key", key } } ); 
                throw;
            }
        }
    }
    catch(GeneralException & ex) 
    {
        ex.AppendInfo({ {"location", "OptionMap"} });
        throw;
    }
}


bool OptionMap::ValidatePy(const std::string & key, const boost::python::object & obj) const
{
    return GetOrThrow_(key)->ValidatePy(obj);
}


bool OptionMap::ValidatePy(const boost::python::dict & opt) const
{
    boost::python::list keys = opt.keys();
    int keylen = boost::python::extract<int>(keys.attr("__len__")());

    try {
        for(int i = 0; i < keylen; i++)
        {
            std::string key = ConvertToCpp<std::string>(keys[i]);

            if(!GetOrThrow_(key)->ValidatePy(opt[key]))
                return false;
        }
    }
    catch(bpmodule::exception::GeneralException & ex)
    {
        ex.AppendInfo({ { "location", "OptionMap::ValidatePy" } });
    }

    return true;
}



void OptionMap::ChangePy(const std::string & key, const boost::python::object & obj)
{
    detail::OptionBasePtr & ptr = GetOrThrow_(key);

    try{
        ptr->ChangeValuePy(obj);
    }
    catch(bpmodule::exception::GeneralException & ex)
    {
        ex.AppendInfo({ { "key", key} });
    }
}


void OptionMap::ChangePy(const boost::python::dict & opt)
{
    boost::python::list keys = opt.keys();
    int keylen = boost::python::extract<int>(keys.attr("__len__")());

    // Check values first
    // (strong exception)
    try {
        for(int i = 0; i < keylen; i++)
        {
            std::string key = ConvertToCpp<std::string>(keys[i]);

            if(!opmap_.count(key))
                throw exception::MapException("Key not found for merging", "OptionMap", key); 

            if(!GetOrThrow_(key)->ValidatePy(opt[key]))
            {
                exception::OptionException ex("Option is invalid", "");
                ex.AppendInfo( { { "key", key } } );
                throw ex;
            }
        }
    }
    catch(PythonConvertException & ex) // catch these, let others pass 
    {
        // should always be a PythonConvertException?
        // Don't catch the MapException, let that go through
        ex.AppendInfo({ {"location", "OptionMap"} });
        throw;
    }

    // now merge
    for(int i = 0; i < keylen; i++)
    {
        std::string key = ConvertToCpp<std::string>(keys[i]);
        ChangePy(key, opt[key]);
    }
}


} // close namespace datastore
} // close namespace bpmodule

