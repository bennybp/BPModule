/*! \file
 *
 * \brief An exception for module loading errors
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_MODULELOADEXCEPTION_HPP_
#define _GUARD_MODULELOADEXCEPTION_HPP_

#include "bpmodule/exception/GeneralException.hpp"

namespace bpmodule {
namespace exception {


/*! \brief An exception thrown when a module couldn't be loaded
 *
 * \todo Update with new scheme
 */
class ModuleLoadException : public GeneralException
{
    public:
        /*! \brief Constructor
         *
         * \param [in] what Brief description of the error
         * \param [in] path Path to the module that could't be loaded
         * \param [in] key  Key of the module that couldn't be loaded 
         * \param [in] name Name of the module that couldn't be loaded 
         * \param [in] desc Additional information (ie, dlerror() or something)
         */
        ModuleLoadException(std::string what,
                            std::string path,
                            std::string key,
                            std::string name,
                            std::string desc = "(no details)")
            : GeneralException(what, "path", path, "key", key, "name", name, "desc", desc) 
        { }
                
        
        ModuleLoadException()                                            = delete;     
        ModuleLoadException(const ModuleLoadException & rhs)             = default;     
        ModuleLoadException(ModuleLoadException && rhs)                  = default;     
        ModuleLoadException & operator=(const ModuleLoadException & rhs) = default;     
        ModuleLoadException & operator=(ModuleLoadException && rhs)      = default;     
        virtual ~ModuleLoadException() = default;


        //! Return the path to the module that couldn't be loaded
        const char * Path(void) const noexcept    { return GeneralException::GetField("path"); }

        //! Return the key of the module that couldn't be loaded
        const char * Key(void) const noexcept     { return GeneralException::GetField("key");  }

        //! Return the name of the module that couldn't be loaded
        const char * Name(void) const noexcept    { return GeneralException::GetField("name"); }

        //! Return any additional details
        const char * Desc(void) const noexcept    { return GeneralException::GetField("desc"); }
};



}// close namespace exception
}// close namespace bpmodule



#endif
