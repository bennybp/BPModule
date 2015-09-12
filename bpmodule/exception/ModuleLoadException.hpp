#ifndef _GUARD_MODULELOADEXCEPTION_HPP_
#define _GUARD_MODULELOADEXCEPTION_HPP_

#include "bpmodule/exception/GeneralException.hpp"

namespace bpmodule {
namespace exception {


class ModuleLoadException : public GeneralException
{
    public:
        ModuleLoadException(std::string what,
                            std::string path,
                            std::string key,
                            std::string name,
                            std::string desc = "(no details)")
            : GeneralException(what,
                               {
                                  {"path", path},
                                  {"key", key},
                                  {"name", name},
                                  {"desc", desc}
                               })
                { }
                
        
        ModuleLoadException()                                            = delete;     
        ModuleLoadException(const ModuleLoadException & rhs)             = default;     
        ModuleLoadException(ModuleLoadException && rhs)                  = default;     
        ModuleLoadException & operator=(const ModuleLoadException & rhs) = default;     
        ModuleLoadException & operator=(ModuleLoadException && rhs)      = default;     
        virtual ~ModuleLoadException() = default;


        const char * Path(void) const noexcept    { return GeneralException::GetField("path"); }
        const char * Key(void) const noexcept     { return GeneralException::GetField("key");  }
        const char * Name(void) const noexcept    { return GeneralException::GetField("name"); }
        const char * Desc(void) const noexcept    { return GeneralException::GetField("desc"); }
};



}// close namespace bpmodule
}// close namespace exception



#endif
