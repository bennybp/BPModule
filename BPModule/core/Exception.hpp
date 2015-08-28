#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdexcept>

#include <vector>
#include <string>
#include <utility>

#include "BPModule/core/Output.hpp"


namespace bpmodule {

class BPModuleException : public std::exception
{
    public:
        typedef std::pair<std::string, std::string> ExceptionInfoPair;
        typedef std::vector<ExceptionInfoPair> ExceptionInfo;

        BPModuleException(std::string whatstr, ExceptionInfo exinfo = {})
            : whatstr_(std::move(whatstr)), exinfo_(std::move(exinfo))
        { }

        const ExceptionInfo GetInfo(void) const
        {
            return exinfo_;
        }

        void AppendInfo(const ExceptionInfo & toappend)
        {
            exinfo_.insert(exinfo_.end(), toappend.begin(), toappend.end());
        }

        const char * what(void) const noexcept
        {
            return whatstr_.c_str();
        }

    private:
        std::string whatstr_;
        ExceptionInfo exinfo_;
};



std::string ExceptionString(const BPModuleException & ex);



namespace export_python {

void TranslateException(const BPModuleException & ex);

} // close namespace export_python




} // close namespace bpmodule
#endif
