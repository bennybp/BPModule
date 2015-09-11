/*! \file
 *
 * \brief Main module database class (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include <boost/python.hpp>

#include "bpmodule/modulestore/ModuleStore.hpp"
#include "bpmodule/output/Output.hpp"
#include "bpmodule/modulebase/ModuleBase.hpp"

using bpmodule::datastore::OptionMap;
using bpmodule::modulebase::ModuleBase;
using bpmodule::exception::GeneralException;


namespace bpmodule {
namespace modulestore {



ModuleStore::ModuleStore()
    : curid_(0)
{ }



void ModuleStore::AddModule(const std::string & key, ModuleGeneratorFunc func, ModuleRemoverFunc dfunc, const ModuleInfo & minfo)
{
    // add to store
    // but throw if key already exists
    if(store_.count(key))
        throw GeneralException(
                                 "Attempt to add duplicate key",
                                 {
                                     { "Location", "ModuleStore"},
                                     { "Key", key }
                                 }
                               );

    store_.emplace(key, StoreEntry{minfo, func, dfunc});
}



void ModuleStore::SetOptions(const std::string & key, const OptionMap & opt)
{
    if(!Has(key))
        throw GeneralException(
                                 "Attempt to set options for nonexistant key",
                                 {
                                     { "Location", "ModuleStore"},
                                     { "Key", key }
                                 }
                               );
   

    ModuleStore::StoreEntry & se = store_.at(key); 
    se.mi.options = opt;    
}


void ModuleStore::SetOptions(const std::string & key, const boost::python::list & opt)
{
    SetOptions(key, OptionMap(opt)); // calls conversion constructor for OptionMap
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


void ModuleStore::DeleteModule_(ModuleBase * mb)
{
    DeleteModule_(mb->ID());
}

void ModuleStore::DeleteModule_(unsigned long id)
{
    if(removemap_.count(id))
    {
        ModuleRemoverFunc dfunc = removemap_.at(id);
        removemap_.erase(id);  // should always happen

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
        throw GeneralException(
                                 "Missing key",
                                 {
                                     { "Location", "ModuleStore"},
                                     { "Key", key }
                                 }
                               );
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
