#ifndef _GUARD_GENERALEXCEPTION_HPP_
#define _GUARD_GENERALEXCEPTION_HPP_

#include <stdexcept>

#include <vector>
#include <string>
#include <utility>


namespace bpmodule {
namespace exception {

class GeneralException : public std::exception
{
    public:
        typedef std::pair<std::string, std::string> ExceptionInfoPair;
        typedef std::vector<ExceptionInfoPair> ExceptionInfo;


        GeneralException(std::string whatstr, ExceptionInfo exinfo = {});

        GeneralException()                                         = delete;
        GeneralException(const GeneralException & rhs)             = default;
        GeneralException(GeneralException && rhs)                  = default;
        GeneralException & operator=(GeneralException && rhs)      = default;
        GeneralException & operator=(const GeneralException & rhs) = default;
        virtual ~GeneralException()                                = default;


        const ExceptionInfo & GetInfo(void) const noexcept;

        const char * GetField(const std::string & field) const noexcept;

        void AppendInfo(const ExceptionInfo & toappend);

        const char * what(void) const noexcept;

        std::string ExceptionString(void) const;

    private:
        std::string whatstr_;
        ExceptionInfo exinfo_;
};


} // close namespace exception
} // close namespace bpmodule
#endif
