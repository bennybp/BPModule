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
    std::cout << "Size: " << store_.size() << "\n";
    for(const auto & it : store_)
    {
        std::cout << "---------------------------------\n"
                  << it.first << "\n"
                  << "---------------------------------\n"
                  << "    Name: " << it.second.name << "\n"
                  << " Version: " << it.second.version << "\n"
                  << "    Path: " << it.second.sopath << "\n"
                  << "    Desc: " << it.second.description << "\n";
        if(it.second.authors.size() > 0)
        {
            std::cout << " Authors: " << it.second.authors[0] << "\n";
            for(size_t i = 1; i < it.second.authors.size(); i++)
                std::cout << "          " << it.second.authors[i] << "\n";
        }
        if(it.second.refs.size() > 0)
        {
            std::cout << "    Refs: " << it.second.refs[0] << "\n";
            for(size_t i = 1; i < it.second.refs.size(); i++)
                std::cout << "          " << it.second.refs[i] << "\n";
        }
        std::cout << " OPTIONS: " << it.second.options.Size() << "\n";
        auto opmap = it.second.options.Dump();
        for(auto & it : opmap)
            std::cout << "    " << it.first << "   =   " << it.second << "\n";
        std::cout << "\n\n";
    }
}


bool ModuleStore::LoadSO(const std::string & key, const std::string & sopath, ModuleInfo minfo)
{
    if(locked_)
    {
        std::cout << "Store is locked. No more loading!\n";
        return false;
    }

    char * error; // for dlerror

    // see if the module is loaded. If so, 
    void * handle;

    if(handles_.count(sopath) == 0)
    {
        std::cout << "Looking to open so file: " << sopath << "\n";
        handle = dlopen(sopath.c_str(), RTLD_NOW | RTLD_GLOBAL);
        // open the module
        if(!handle)
        {
            std::cout << "Error - unable to open SO file: " << sopath << "\n";
            error = dlerror();
            std::cout << error << "\n";
            return false;
        }
        std::cout << "Successfully opened " << sopath << "\n";
    }
    else
        handle = handles_[sopath];

    typedef ModuleBase * (*getptr)(const std::string &, ModuleStore *, const OptionMap &);

    getptr fn = reinterpret_cast<getptr>(dlsym(handle, "CreateModule"));
    if((error = dlerror()) != NULL)
    {
        std::cout << "Error - unable to find CreateModule!\n";
        std::cout << error << "\n";
        dlclose(handle);
        return false;
    }
        
    minfo.createfunc = std::bind(fn, key, std::placeholders::_1, std::placeholders::_2);
    minfo.sopath = sopath;

    // add to store
    //! \todo Check for duplicates
    store_[key] = minfo;
    if(handles_.count(sopath) == 0)
        handles_[sopath] = handle;

    return true;
}

void ModuleStore::CloseAll(void)
{
    store_.clear();
    for(auto it : handles_)
    {
        std::cout << "Closing " << it.first << "\n";
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
}


ModuleStore::~ModuleStore()
{
    std::cout << "MODULESTORE DESTRUCTOR\n";
    CloseAll();
}


} // close namespace bpmodule
