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
        auto & minfo = it.second.first;   // it.second = a pair<ModuleInfo, Func>
        std::cout << "---------------------------------\n"
                  << it.first << "\n"
                  << "---------------------------------\n"
                  << "    Name: " << minfo.name << "\n"
                  << " Version: " << minfo.version << "\n"
                  << "    Path: " << minfo.sopath << "\n"
                  << "    Desc: " << minfo.description << "\n";
        if(minfo.authors.size() > 0)
        {
            std::cout << " Authors: " << minfo.authors[0] << "\n";
            for(size_t i = 1; i < minfo.authors.size(); i++)
                std::cout << "          " << minfo.authors[i] << "\n";
        }
        if(minfo.refs.size() > 0)
        {
            std::cout << "    Refs: " << minfo.refs[0] << "\n";
            for(size_t i = 1; i < minfo.refs.size(); i++)
                std::cout << "          " << minfo.refs[i] << "\n";
        }
        std::cout << " OPTIONS: " << minfo.options.Size() << "\n";
        auto opmap = minfo.options.Dump();
        for(auto & it2 : opmap)
            std::cout << "    " << it2.first << "   =   " << it2.second << "\n";
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

    typedef ModuleBase * (*getptr)(const std::string &, long, ModuleStore *, const OptionMap &);

    getptr fn = reinterpret_cast<getptr>(dlsym(handle, "CreateModule"));
    if((error = dlerror()) != NULL)
    {
        std::cout << "Error - unable to find CreateModule!\n";
        std::cout << error << "\n";
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
    curid_ = 0;
}


ModuleStore::~ModuleStore()
{
    std::cout << "MODULESTORE DESTRUCTOR\n";
    CloseAll();
}


} // close namespace bpmodule
