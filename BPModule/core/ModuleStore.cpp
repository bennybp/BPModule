#include "BPModule/core/Exception.hpp"
#include "BPModule/core/ModuleStore.hpp"
#include "BPModule/core/Output.hpp"
#include "BPModule/core/ModuleBase.hpp"


namespace bpmodule {


void ModuleStore::AddModule(const std::string & key, ModuleGeneratorFunc func, ModuleRemoverFunc dfunc, const ModuleInfo & minfo)
{
    // add to store
    // but throw if key already exists
    if(Has(key))
        throw BPModuleException(
                                 "Attempt to add duplicate key",
                                 {
                                     { "Location", "ModuleStore"},
                                     { "Key", key }
                                 }
                               );

    store_.insert(StoreMap::value_type(key, StoreEntry {minfo, func, dfunc}));
}



void ModuleStore::SetOptions(const std::string & key, const OptionMap & opt)
{
    if(!Has(key))
        throw BPModuleException(
                                 "Attempt to set options for nonexistant key",
                                 {
                                     { "Location", "ModuleStore"},
                                     { "Key", key }
                                 }
                               );
   

    ModuleStore::StoreEntry & se = store_.at(key); 
    se.mi.options = opt;    
}



size_t ModuleStore::Size(void) const
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


void ModuleStore::RemoveModule(unsigned long id)
{
    if(removemap_.count(id))
    {
        removemap_[id](id);
        removemap_.erase(id);
    }
}

void ModuleStore::RemoveModule(ModuleBase * mb)
{
    RemoveModule(mb->ID());
}


ModuleStore::ModuleStore()
{
    curid_ = 0;
}



const ModuleStore::StoreEntry & ModuleStore::GetOrThrow_(const std::string & key) const
{
    if(Has(key))
        return store_.at(key);
    else
        throw BPModuleException(
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


} // close namespace bpmodule
