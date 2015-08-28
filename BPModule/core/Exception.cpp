#include <sstream>

#include "BPModule/core/Exception.hpp"


namespace bpmodule {

std::string ExceptionString(const BPModuleException & ex)
{
    BPModuleException::ExceptionInfo exinfo = ex.GetInfo();
    std::stringstream ss;
    bpmodule::output::Output(ss, bpmodule::output::Line('*'));
    bpmodule::output::Output(ss, "Exception thrown!\n");
    bpmodule::output::Output(ss, "what() = %1%\n", ex.what());
    for(auto & it : exinfo)
        if(it.second.size())
            bpmodule::output::Output(ss, "%|24| : %|-|\n", it.first, it.second);
    return ss.str();
}



} // close namespace bpmodule
