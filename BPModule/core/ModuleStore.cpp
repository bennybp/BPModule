#include "BPModule/core/Exception.h"
#include "BPModule/core/ModuleStore.h"
#include "BPModule/core/Output.h"
#include "BPModule/core/ModuleBase.h"


namespace bpmodule {


bool ModuleStore::AddCreateFunc(const std::string & key, ModuleGeneratorFunc func, ModuleDeleterFunc dfunc, const ModuleInfo & minfo)
{
    // add to store
    //! \todo Check for duplicates
    store_.insert(StoreMap::value_type(key, StoreEntry {minfo, func, dfunc}));
    return true;
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



ModuleInfo ModuleStore::ModuleInfoFromKey(const std::string & key) const
{
    return GetOrThrow_(key).mi;
}



void ModuleStore::Lock(void)
{
    locked_ = true;
}



void ModuleStore::Delete(unsigned long id)
{
    if(deletemap_.count(id))
    {
        deletemap_[id](id);
        deletemap_.erase(id);
    }
}

void ModuleStore::Delete(ModuleBase * mb)
{
    Delete(mb->ID());
}


ModuleStore::ModuleStore()
{
    curid_ = 0;
    locked_ = false;
}



const ModuleStore::StoreEntry & ModuleStore::GetOrThrow_(const std::string & key) const
{
    if(store_.count(key))
        return store_.at(key);
    else
        throw MapException("ModuleStore::StoreMap", key);
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
