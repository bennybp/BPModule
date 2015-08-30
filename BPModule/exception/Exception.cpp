#include <sstream>

#include "BPModule/exception/Exception.hpp"
#include "BPModule/output/Output.hpp"


namespace bpmodule {

BPModuleException::BPModuleException(std::string whatstr, ExceptionInfo exinfo)
    : whatstr_(std::move(whatstr)), exinfo_(std::move(exinfo))
{ }


const BPModuleException::ExceptionInfo BPModuleException::GetInfo(void) const
{
    return exinfo_;
}



void BPModuleException::AppendInfo(const ExceptionInfo & toappend)
{
    exinfo_.insert(exinfo_.end(), toappend.begin(), toappend.end());
}



const char * BPModuleException::what(void) const noexcept
{
    return whatstr_.c_str();
}



std::string BPModuleException::ExceptionString(void) const
{
    ExceptionInfo exinfo = GetInfo();
    std::stringstream ss;
    output::Output(ss, output::Line('*'));
    output::Output(ss, "Exception thrown!\n");
    output::Output(ss, "what() = %1%\n", what());
    for(auto & it : exinfo)
        if(it.second.size())
            output::Output(ss, "%|24| : %|-|\n", it.first, it.second);
    return ss.str();
}


} // close namespace bpmodule
