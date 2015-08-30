#include <sstream>

#include "BPModule/exception/GeneralException.hpp"
#include "BPModule/output/Output.hpp"


namespace bpmodule {
namespace exception {

GeneralException::GeneralException(std::string whatstr, ExceptionInfo exinfo)
    : whatstr_(std::move(whatstr)), exinfo_(std::move(exinfo))
{ }


const GeneralException::ExceptionInfo GeneralException::GetInfo(void) const
{
    return exinfo_;
}



void GeneralException::AppendInfo(const ExceptionInfo & toappend)
{
    exinfo_.insert(exinfo_.end(), toappend.begin(), toappend.end());
}



const char * GeneralException::what(void) const noexcept
{
    return whatstr_.c_str();
}



std::string GeneralException::ExceptionString(void) const
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


} // close namespace exception
} // close namespace bpmodule
