#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <boost/lexical_cast.hpp>

#include <dlfcn.h>

#include "BPModule/core/Exception.h"
#include "BPModule/core/ModuleStore.h"
#include "BPModule/core/Output.h"

#include "BPModule/core/ModuleBase.h"


namespace bpmodule {


bool ModuleStore::LoadSO(const std::string & key, const std::string & sopath, ModuleInfo minfo)
{
    typedef ModuleBase * (*getptr)(const std::string &, unsigned long, ModuleStore *, const OptionMap &);

    if(locked_)
    {
        Warning("Store is locked. No more loading!\n");
        return false;
    }

    char * error; // for dlerror
    void * handle;

    // see if the module is loaded
    // if so, reuse that handle
    if(handles_.count(sopath) > 0)
        handle = handles_[sopath];
    else
    {
        Output("Looking to open so file: %1%\n", sopath);
        handle = dlopen(sopath.c_str(), RTLD_NOW | RTLD_GLOBAL);
        // open the module
        if(!handle)
        {
            Error("Unable to open SO file: %1%\n", sopath);
            error = dlerror();
            Error("%1%\n", error);
            return false;
        }
    }


    // get the pointer to the CreateModule function
    getptr fn = reinterpret_cast<getptr>(dlsym(handle, "CreateModule"));
    if((error = dlerror()) != NULL)
    {
        Error("Unable to find CreateModule!\n");
        Error("%1%\n", error);
        dlclose(handle);
        return false;
    }
        
    ModuleGeneratorFunc createfunc = std::bind(fn, key, std::placeholders::_1,
                                                        std::placeholders::_2,
                                                        std::placeholders::_3);
    minfo.sopath = sopath;

    // add to store
    //! \todo Check for duplicates
    store_.insert(StoreMapValue(key, StoreEntry{minfo, createfunc}));

    if(handles_.count(sopath) == 0)
        handles_.insert(HandleMapValue(sopath, handle));

    Success("Successfully opened %1%\n", sopath);
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



ModuleInfo ModuleStore::ModuleInfoFromID(unsigned long id) const
{
    const std::string & key = GetOrThrow(id);
    return GetOrThrow(key).mi;
}



ModuleInfo ModuleStore::ModuleInfoFromKey(const std::string & key) const
{
    return GetOrThrow(key).mi;
}


  
std::string ModuleStore::KeyFromID(unsigned long id) const
{
    return GetOrThrow(id);
}



void ModuleStore::CloseAll(void)
{
    store_.clear();
    for(auto it : handles_)
    {
        Output("Closing %1%\n", it.first);
        dlclose(it.second);
    }
    handles_.clear();
}



void ModuleStore::Lock(void)
{
    locked_ = true;
}



ModuleStore::ModuleStore()
{
    locked_ = false;
    curid_ = 0;
}



const ModuleStore::StoreEntry & ModuleStore::GetOrThrow(const std::string & key) const
{
    if(store_.count(key))
        return store_.at(key);
    else
        throw MapException("ModuleStore::StoreMap", key);
}



const std::string & ModuleStore::GetOrThrow(unsigned long id) const
{
    if(idmap_.count(id))
        return idmap_.at(id);
    else
        throw MapException("ModuleStore::IDMap", boost::lexical_cast<std::string>(id));
}



ModuleStore::~ModuleStore()
{
    CloseAll();
}


} // close namespace bpmodule
