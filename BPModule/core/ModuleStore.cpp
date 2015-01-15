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
#include "BPModule/core/ModuleBase.h"
#include "BPModule/core/Output.h"


namespace bpmodule {


size_t ModuleStore::Size(void) const
{
    return store_.size();
}

void ModuleStore::PrintHelp(const std::string & key) const
{
    if(Has(key))
    {
        const StoreEntry & se = GetOrThrow(key);
        se.mi.PrintHelp();
    }
    else
        Warning("Cannot find module \"%1%\". Not loaded?\n", key);
}

void ModuleStore::PrintInfo(const std::string & key) const
{
    if(Has(key))
    {
        const StoreEntry & se = GetOrThrow(key);
        se.mi.PrintInfo();
    }
    else
        Warning("Cannot find module \"%1%\". Not loaded?\n", key);
}

void ModuleStore::PrintHelp(void) const
{
    Output(Line('*'));
    for(const auto & it : store_)
    {
        Output(Line('-'));
        Output("KEY: %1%\n", it.first);
        Output(Line('-'));
        PrintHelp(it.first);
    }
}

void ModuleStore::PrintInfo(void) const
{
    Output(Line('*'));
    Output("Module store: Size: %1%\n", store_.size());
    for(const auto & it : store_)
    {
        Output(Line('-'));
        Output("KEY: %1%\n", it.first);
        Output(Line('-'));
        PrintInfo(it.first);
    }
    Output(Line('*'));
}

void ModuleStore::PrintKeys(void) const
{
    Output(Line('*'));
    Output("Module store: Size: %1%\n", store_.size());
    for(const auto & it : store_)
        Output("%1%\n", it.first);
    Output(Line('*'));
}

std::vector<std::string> ModuleStore::Keys(void) const
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

bool ModuleStore::LoadSO(const std::string & key, const std::string & sopath, ModuleInfo minfo)
{
    typedef ModuleBase * (*getptr)(const std::string &, long, ModuleStore *, const OptionMap &);

    if(locked_)
    {
        Warning("Store is locked. No more loading!\n");
        return false;
    }

    char * error; // for dlerror

    // see if the module is loaded. If so, 
    void * handle;

    if(handles_.count(sopath) == 0)
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
    else
        handle = handles_[sopath];


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
    store_.insert(StoreMapValue(key, StoreEntry({minfo, createfunc})));

    if(handles_.count(sopath) == 0)
        handles_.insert(HandleMapValue(sopath, handle));

    Success("Successfully opened %1%\n", sopath);
    return true;
}

ModuleInfo ModuleStore::ModuleInfoFromID(long id) const
{
    const std::string & key = GetOrThrow(id);
    return GetOrThrow(key).mi;
}

ModuleInfo ModuleStore::ModuleInfoFromKey(const std::string & key) const
{
    return GetOrThrow(key).mi;
}
  
std::string ModuleStore::KeyFromID(long id) const
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

const std::string & ModuleStore::GetOrThrow(long id) const
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
