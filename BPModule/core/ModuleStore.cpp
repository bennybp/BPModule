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


namespace bpmodule {


size_t ModuleStore::Size(void) const
{
    return store_.size();
}

void ModuleStore::Help(const std::string & key) const
{
    const StoreEntry & se = GetOrThrow(key);
    se.mi.Help(*out_);
}

void ModuleStore::Info(void) const
{
    (*out_) << "Size: " << store_.size() << "\n";
    for(const auto & it : store_)
    {
        (*out_) << "---------------------------------\n"
                  << it.first << "\n"
                  << "---------------------------------\n";
        it.second.mi.Info(*out_);
    }
}


bool ModuleStore::LoadSO(const std::string & key, const std::string & sopath, ModuleInfo minfo)
{
    typedef ModuleBase * (*getptr)(const std::string &, long, ModuleStore *, const OptionMap &);

    if(locked_)
    {
        (*out_) << "Store is locked. No more loading!\n";
        return false;
    }

    char * error; // for dlerror

    // see if the module is loaded. If so, 
    void * handle;

    if(handles_.count(sopath) == 0)
    {
        (*out_) << "Looking to open so file: " << sopath << "\n";
        handle = dlopen(sopath.c_str(), RTLD_NOW | RTLD_GLOBAL);
        // open the module
        if(!handle)
        {
            (*out_) << "Error - unable to open SO file: " << sopath << "\n";
            error = dlerror();
            (*out_) << error << "\n";
            return false;
        }
        (*out_) << "Successfully opened " << sopath << "\n";
    }
    else
        handle = handles_[sopath];


    getptr fn = reinterpret_cast<getptr>(dlsym(handle, "CreateModule"));
    if((error = dlerror()) != NULL)
    {
        (*out_) << "Error - unable to find CreateModule!\n";
        (*out_) << error << "\n";
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
        (*out_) << "Closing " << it.first << "\n";
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
    out_ = &std::cout;
}

std::ostream & ModuleStore::GetOutput(void) const
{
    return *out_;
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
    (*out_) << "MODULESTORE DESTRUCTOR\n";
    CloseAll();
}


} // close namespace bpmodule
