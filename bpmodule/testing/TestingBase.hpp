#ifndef _GUARD_TESTINGBASE_HPP_
#define _GUARD_TESTINGBASE_HPP_

#include <functional>

#include "bpmodule/output/Output.hpp"

using bpmodule::output::Output;
using bpmodule::output::Error;

namespace bpmodule {
namespace testing {

template<typename T, typename... Targs>
int TestFunc(int itest, const std::string & desc, bool expected, T func, Targs... Fargs)
{
    std::string fmt("%1$5% : %2$9%  : %3%\n");

    try {
       func(Fargs...); 
    }
    catch(...)
    {
        if(expected != false)
        {
            Error(fmt, itest, "FAILED", desc);
            return 1;
        }
        else
        {
            Output(fmt, itest, "Success", desc);
            return 0;
        }
        
    }

    if(expected != true)
    {
        Error(fmt, itest, "FAILED", desc);
        return 1;
    }
    else
    {
        Output(fmt, itest, "Success", desc);
        return 0;
    }

}



template<typename T, typename... Targs>
int TestConstruct(int itest, const std::string & desc, bool expected, Targs... Fargs)
{
    std::string fmt("%|1$5| : %|2$-9|  : %3%\n");

    try {
       T obj(Fargs...); 
    }
    catch(...)
    {
        if(expected == false)
        {
            Output(fmt, itest, "Success", desc);
            return 0;
        }
        else
        {
            Error(fmt, itest, "FAILED", desc);
            return 1;
        }
        
    }

    if(expected == true)
    {
        Output(fmt, itest, "Success", desc);
        return 0;
    }
    else
    {
        Error(fmt, itest, "FAILED", desc);
        return 1;
    }

}



} // close namespace testing
} // close namespace bpmodule

#endif
