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


bool ModuleStore::Merge(const std::string & filepath, void * handle, const StoreType & st)
{
    if(handles_.count(filepath) > 0)
    {
        std::cout << "Cannot load the same module twice! Module: " << filepath << "\n";
        dlclose(handle);
        return false;
    }
    
    for(auto & it : st)
    {
        if(store_.count(it.first) > 0)
        {
            std::cout << "Duplicate module: " << it.first << "\n";
            dlclose(handle);
            return false;
        }

        // \todo check for stuff
    }

    store_.insert(st.begin(), st.end());    

    for(auto & it : st)
        sofiles_.insert(SOFileMap::value_type(it.first, filepath));

    handles_.insert(HandleMap::value_type(filepath, handle));

    return true;
}


size_t ModuleStore::Count(void) const
{
    return store_.size();
}

void ModuleStore::DumpInfo(void) const
{
    std::cout << "Count: " << store_.size() << "\n";
    for(const auto & it : store_)
    {
        std::cout << "---------------------------------\n"
                  << it.first << "\n"
                  << "---------------------------------\n"
                  << "    Name: " << it.second.name << "\n"
                  << " Version: " << it.second.version << "\n"
                  << "    Path: " << sofiles_.at(it.first) << "\n"
                  << "   Class: " << MClassToString(it.second.mclass) << "\n"
                  << "    Type: " << MTypeToString(it.second.mtype) << "\n"
                  << " Authors: " << it.second.authors << "\n"
                  << "    Desc: " << it.second.description << "\n"
                  << "    Refs: " << it.second.refs << "\n"
                  << " OPTIONS: " << it.second.options.Count() << "\n";
        auto opmap = it.second.options.DumpMap();
        for(auto & it : opmap)
            std::cout << "    " << it.first << "   =   " << it.second << "\n";
        std::cout << "\n\n";
    }
}


bool ModuleStore::SelectComponents(const std::string & components, StoreType & st)
{
    // this whole function is ugly


    // Tokenize the component string
    std::vector<std::string> compadd, compdel;
    std::stringstream ss(components);
    std::string tok;
    while(std::getline(ss, tok, ' '))
    {
        if(tok.length() > 0)
        {
            if(tok[0] == '-')
                compdel.push_back(tok.substr(1, std::string::npos));
            else if(tok[0] == '+')
                compadd.push_back(tok.substr(1, std::string::npos));
            else
                compadd.push_back(tok.substr(0, std::string::npos));
        }
    }
    

    std::cout << "To add:\n";
    for(auto & it : compadd)
        std::cout << "'" << it << "'\n";
    std::cout << "To remove:\n";
    for(auto & it : compdel)
        std::cout << "'" << it << "'\n";

    // 1.) Check to make sure all add and del components are available
    for(auto & it : compadd)
    {
        if(st.count(it) == 0)
        {
            std::cout << "Component " << it << " not provided by module!\n";
            return false;
        }
    }
    for(auto & it : compdel)
    {
        if(st.count(it) == 0)
        {
            std::cout << "Component " << it << " not provided by module!\n";
            return false;
        }
    }


    // 2.) Make sure if compadd and compdel are separate (if both specified)
    //     and only add
    //! \todo check for duplicates
    StoreType toadd;
    if(compadd.size() == 0 && compdel.size() == 0)
        toadd = st;  // add them all
    else if(compadd.size() > 0 && compdel.size() > 0)
    {
        // just check if any are in both and return false if so
        for(auto & it : compadd)
        {
            auto findit = std::find_if(compdel.begin(),
                                       compdel.end(),
                                       [it](const std::string & st) { return st == it; } );
            if(findit != compdel.end())
            {
                std::cout << "Can't both add and remove a component from a module!\n";
                return false;
            }

            // there's probably to be a better way to do this
            for(auto & it : compadd)
                toadd[it] = st[it];
        }
    }

    // 3.) There are only adds 
    else if(compdel.size() == 0)
    {
        for(auto & it : compadd)
            toadd[it] = st[it];
    }
    else if(compadd.size() == 0)
    {
        toadd = st;
        // only want to delete some
        for(auto & it : compdel)
           toadd.erase(it); 
    }

    // Do we have anything to add?
    if(toadd.size() == 0)
    {
        std::cout << "Umm.... This module adds nothing?\n";
        return false;
    }

    // store the results
    st = toadd;
    return true;
}


bool ModuleStore::LoadSO(const char * modulepath, const char * components)
{
    if(locked_)
    {
        std::cout << "Store is locked. No more loading!\n";
        return false;
    }

    //! \todo Wrap handle obeject in a smart-ptr type thing
    //        Also store that in the handle map
    typedef StoreType (*getptr)(void);

    char * error;

    // open the module
    std::cout << "Looking to open so file: " << modulepath << "\n";
    void * handle = dlopen(modulepath, RTLD_NOW | RTLD_GLOBAL);
    if(!handle)
    {
        std::cout << "Error - unable to open SO file: " << modulepath << "\n";
        error = dlerror();
        std::cout << error << "\n";
        return false;
    }
    else
        std::cout << "Successfully opened " << modulepath << "\n";


    getptr fn = reinterpret_cast<getptr>(dlsym(handle, "GetComponents"));
    if((error = dlerror()) != NULL)
    {
        std::cout << "Error - unable to find GetComponents!\n";
        std::cout << error << "\n";
        dlclose(handle);
        return false;
    }
        
    StoreType st = fn();
    std::cout << "Components available:\n";
    for(auto & it : st)
        std::cout << "  " << it.first << "\n";

    if(SelectComponents(components, st) == false)
    {
        dlclose(handle);
        return false;
    }

    // Add the components
    if(Merge(modulepath, handle, st) == false)
    {
        std::cout << "Error adding module " << modulepath << "\n";
        return false;
    }

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
    CloseAll();
}


} // close namespace bpmodule
