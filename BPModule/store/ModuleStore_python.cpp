#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <sstream>
#include <algorithm>

#include <dlfcn.h>

#include "BPModule/store/ModuleStore.h"

using namespace bpmodule;


namespace {
  ModuleStore mstore_;
}

extern "C" {


bool LoadModule(const char * modulepath, const char * components)
{
    typedef StoreType (*getptr)(void);

    char * error;

    // open the module
    std::cout << "Looking to open so file: " << modulepath << "\n";
    void * handle = dlopen(modulepath, RTLD_LAZY);
    if(!handle)
    {
        std::cout << "Error - unable to open SO file: " << modulepath << "\n";
        return false;
    }
    else
        std::cout << "Successfully opened " << modulepath << "\n";


    getptr fn = reinterpret_cast<getptr>(dlsym(handle, "GetComponents"));
    if((error = dlerror()) != NULL)
    {
        std::cout << "Error - unable to find GetComponents!\n";
        std::cout << error << "\n";
        return false;
    }
        
    StoreType st = fn();
    std::cout << "Components available:\n";
    for(auto & it : st)
        std::cout << "  " << it.first << "\n";


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
            std::cout << "Component " << it << " not provided by module " << modulepath << "\n";
            return false;
        }
    }
    for(auto & it : compdel)
    {
        if(st.count(it) == 0)
        {
            std::cout << "Component " << it << " not provided by module " << modulepath << "\n";
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

            // there's got to be a better way to do this
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

    // Add the components
    if(mstore_.Merge(modulepath, toadd) == false)
    {
        std::cout << "Error adding module " << modulepath << "\n";
        return false;
    }

    return true;

}


int Count(void)
{
    return static_cast<int>(mstore_.Count());
}


void DumpInfo(void)
{
    mstore_.DumpInfo();
}


} // close extern C
