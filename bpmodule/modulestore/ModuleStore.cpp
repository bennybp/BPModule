/*! \file
 *
 * \brief Main module database class (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include <boost/python.hpp>

#include "bpmodule/modulestore/ModuleStore.hpp"
#include "bpmodule/output/Output.hpp"
#include "bpmodule/modulebase/ModuleBase.hpp"
#include "bpmodule/exception/ModuleLoadException.hpp"

using bpmodule::datastore::OptionMap;
using bpmodule::modulebase::ModuleBase;
using bpmodule::exception::ModuleLoadException;
using bpmodule::exception::ModuleCreateException;
using bpmodule::exception::GeneralException;


namespace bpmodule {
namespace modulestore {



ModuleStore::ModuleStore()
    : curid_(0)
{ }



void ModuleStore::InsertModule(const std::string & key, ModuleGeneratorFunc func,
                               ModuleRemoverFunc dfunc, const ModuleInfo & minfo)
{
    // add to store
    // but throw if key already exists
    if(store_.count(key))
        throw ModuleLoadException("Cannot insert module: duplicate key",
                                  minfo.path, key, minfo.name);

    store_.emplace(key, StoreEntry{minfo, func, dfunc});
}


void ModuleStore::SetOptions(const std::string & key, const boost::python::dict & opt)
{
    GetOrThrow_(key).mi.options.ChangePyDict(opt);
}


size_t ModuleStore::Size(void) const noexcept
{
    return store_.size();
}


std::vector<std::string> ModuleStore::GetKeys(void) const
{
    std::vector<std::string> vec;
    vec.reserve(store_.size());
    for(const auto & it : store_)
        vec.push_back(it.first);
    return vec;
}



bool ModuleStore::Has(const std::string & key) const
{
    return store_.count(key);
}



ModuleInfo ModuleStore::KeyInfo(const std::string & key) const
{
    return GetOrThrow_(key).mi;
}

void ModuleStore::TestAll(void)
{
    output::Debug("Testing all modules\n");
    for(const auto & it : store_)
    {
        output::Debug("Testing %1% (%2%)...\n", it.first, it.second.mi.name);

        if(!it.second.mi.options.IsValid())
            output::Error("Error - module %1% [key %2%]\" faild options test - options are missing or otherwise invalid", it.second.mi.name, it.first);

        try {
            GetModule<ModuleBase>(it.first);
        }
        catch(...)
        {
            output::Error("Error - module %1% [key %2%]\" failed test loading!\n", it.second.mi.name, it.first);
            throw;
        }

        output::Debug("Test of %1% OK\n", it.first);
    }   
}


void ModuleStore::DeleteObject_(ModuleBase * mb)
{
    DeleteObject_(mb->ID());
}

void ModuleStore::DeleteObject_(unsigned long id)
{
    if(removemap_.count(id))
    {
        ModuleRemoverFunc dfunc = removemap_.at(id);
        removemap_.erase(id);  // should always happen
        minfomap_.erase(id); //! \todo remove after moving to tree

        // throwing ok from here. Stuff has already been removed from this map
        // (but this shouldn't really throw)
        dfunc(id);
    }
}



const ModuleStore::StoreEntry & ModuleStore::GetOrThrow_(const std::string & key) const
{
    if(Has(key))
        return store_.at(key);
    else
        throw GeneralException("Missing module key",
                               "location", "ModuleStore",
                               "modulekey", key);
}


ModuleStore::StoreEntry & ModuleStore::GetOrThrow_(const std::string & key)
{
    if(Has(key))
        return store_.at(key);
    else
        throw GeneralException("Missing module key",
                               "location", "ModuleStore",
                               "modulekey", key);
}


ModuleStore::~ModuleStore()
{
    // it is the responsiblility of the various
    // loaders to be responsible for deleting all
    // the objects. The deletion functors aren't
    // called here since this should be destructed
    // after all the loaders
}

} // close namespace modulestore
} // close namespace bpmodule
