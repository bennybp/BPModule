#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <sstream>

#include <dlfcn.h>

#include "BPModule/store/ModuleStore.h"

using namespace bpmodule;


namespace {
  ModuleStore mstore_;
}

extern "C" {


void LoadModule(const char * modulepath, const char * components)
{
    typedef StorePairVec (*getptr)(void);

    char * error;

    // open the module
    std::cout << "Looking to open so file: " << modulepath << "\n";
    void * handle = dlopen(modulepath, RTLD_LAZY);
    if(!handle)
        throw std::runtime_error("Error - unable to open so file!");
    else
        std::cout << "Successfully opened " << modulepath << "\n";


    getptr fn = reinterpret_cast<getptr>(dlsym(handle, "GetComponents"));
    if((error = dlerror()) != NULL)
    {
        std::cout << error << "\n";
        throw std::runtime_error("Error - unable to find GetComponents!");
    }
        
    StorePairVec spv = fn();
    std::cout << "Components available:\n";
    for(auto & it : spv)
        std::cout << "  " << it.first << "\n";

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


    // TODO do some figgerin'
    StorePairVec spvadd = spv;

    // Add the components
    for(auto & it : spvadd)
        mstore_.Add(it);

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
