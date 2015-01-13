#include <iostream>
#include <stdexcept>
#include <dlfcn.h>

#include "BPModule/core/ModuleStore.h"
#include "BPModule/core/base/Test_Base.h"

using namespace bpmodule;


namespace {
  ModuleStore mstore_;
}


static std::vector<std::string> Char2StarToVec(int n, const char ** arr)
{
    std::vector<std::string> vec(n);
    for(int i = 0; i < n; i++)
        vec[i] = std::string(arr[i]);
    return vec;
}


static OptionMap ParseOptions(int n, const char *** options)
{
    OptionMap op;
    for(int i = 0; i < n; i++)
    {
        std::string type(options[i][1]);
        std::stringstream ss;
        ss << options[i][2];
        ss.exceptions(std::stringstream::failbit | std::stringstream::badbit);

        if(type == "double")
        {
            double d;
            ss >> d;
            op.Set(options[i][0], static_cast<double>(d));
        }
        else if(type == "int")
        {
            int ii;
            ss >> ii;
            op.Set(options[i][0], static_cast<int>(ii));
        }
        else if(type == "bool")
        {
            bool b;
            if(ss.str() == "on")
                b = true;
            else if(ss.str() == "off")
                b = false;
            else
            {
                std::cout << "Unknown bool string: " << ss.str() << "\n";
                throw std::runtime_error("Unknown bool string");
            }
            op.Set(options[i][0], b);
        }
        else if(type == "string")
        {
            op.Set(options[i][0], ss.str());
        }
        else
        {
            std::cout << "Unknown option type: " << type << "\n";
            throw std::runtime_error("Unknown option type");
        }
    }
    return op;
}

extern "C" {

bool LoadSO(const char * modulepath,
            const char * name,
            const char * version,
            const char * mclass,
            const char * mtype,
            const char * description,
            int nauthors, const char ** authors, 
            int nrefs, const char ** refs,
            int noptions, const char *** options)
{
    //for(int i = 0; i < noptions; i++)
    //    std::cout << options[i][0] << " " << options[i][1] << " " << options[i][2] << "\n";
    ModuleInfo minfo;
    minfo.name = name;
    minfo.version = version;
    minfo.mclass = StringToMClass(mclass);
    minfo.mtype = StringToMType(mtype);
    minfo.authors = Char2StarToVec(nauthors, authors);    
    minfo.description = description;
    minfo.refs = Char2StarToVec(nrefs, refs);    
    minfo.options = ParseOptions(noptions, options);
    minfo.sofile = modulepath;

    // handle will be set below

    return mstore_.LoadSO(modulepath, minfo);
}

int Count(void)
{
    return static_cast<int>(mstore_.Count());
}


void DumpInfo(void)
{
    mstore_.DumpInfo();
}

void RunTest(const char * modname)
{
    std::cout << "Test: " << modname << "\n";
    std::unique_ptr<Test_Base> tptr = mstore_.GetModule<Test_Base>(modname);
    tptr->RunTest();
}

void InitParallel(void)
{
}

void FinalizeParallel(void)
{
}

} // close extern C
