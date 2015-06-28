#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdexcept>

#include <fstream>
#include <vector>
#include <string>
#include <utility>



namespace bpmodule {

class BPModuleException : public std::exception
{
    public:
        typedef std::pair<std::string, std::string> ExceptionInfoPair;
        typedef std::vector<ExceptionInfoPair> ExceptionInfo;

        BPModuleException(std::string whatstr, ExceptionInfo exinfo)
            : whatstr_(std::move(whatstr)), exinfo_(std::move(exinfo))
        { }

        BPModuleException(std::string whatstr)
            : whatstr_(std::move(whatstr))
        { }

        const ExceptionInfo GetInfo(void) const
        {
            return exinfo_;
        }

        const char * what(void) const noexcept
        {
            return whatstr_.c_str();
        }

    private:
        std::string whatstr_;
        ExceptionInfo exinfo_;
};


} // close namespace bpmodule
#endif
