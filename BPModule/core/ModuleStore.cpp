#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <sstream>
#include <algorithm>

#include <dlfcn.h>

#include "BPModule/core/ModuleStore.h"
#include "BPModule/core/ModuleBase.h"


namespace bpmodule {


size_t ModuleStore::Size(void) const
{
    return store_.size();
}

void ModuleStore::Dump(void) const
{
    (*out_) << "Size: " << store_.size() << "\n";
    for(const auto & it : store_)
    {
        auto & minfo = it.second.first;   // it.second = a pair<ModuleInfo, Func>
        (*out_) << "---------------------------------\n"
                  << it.first << "\n"
                  << "---------------------------------\n";
        minfo.Dump(*out_);
    }
}


bool ModuleStore::LoadSO(const std::string & key, const std::string & sopath, ModuleInfo minfo)
{
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

    typedef ModuleBase * (*getptr)(const std::string &, long, ModuleStore *, const OptionMap &);

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
    store_[key] = std::pair<ModuleInfo, ModuleGeneratorFunc>(minfo, createfunc);
    if(handles_.count(sopath) == 0)
        handles_[sopath] = handle;

    return true;
}

ModuleInfo ModuleStore::ModuleInfoFromID(long id) const
{
    return store_.at(idmap_.at(id)).first;
}

ModuleInfo ModuleStore::ModuleInfoFromKey(const std::string & key) const
{
    return store_.at(key).first;
}
  
std::string ModuleStore::KeyFromID(long id) const
{
    return idmap_.at(id);
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

void ModuleStore::Help(const std::string & key) const
{
    store_.at(key).first.Help(*out_);
}


ModuleStore::~ModuleStore()
{
    (*out_) << "MODULESTORE DESTRUCTOR\n";
    CloseAll();
}


} // close namespace bpmodule
