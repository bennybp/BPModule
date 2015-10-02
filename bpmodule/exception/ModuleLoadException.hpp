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
         * \param [in] exinfo Additional information. Must be an even number of strings
         */
        template<typename... Targs>
        ModuleLoadException(std::string what,
                            std::string path,
                            std::string key,
                            std::string name,
                            Targs... exinfo)
            : GeneralException(what, "path", path, "key", key, "name", name, exinfo...)
        { }


        /*! \brief Constructor, using another exception as a base
         *
         * Can be used to append additional information
         *
         * \param [in] gex Exception to copy
         * \param [in] path Path to the module that could't be loaded
         * \param [in] key  Key of the module that couldn't be loaded
         * \param [in] name Name of the module that couldn't be loaded
         * \param [in] exinfo Additional information. Must be an even number of strings
         */
        template<typename... Targs>
        ModuleLoadException(const GeneralException & gex,
                            std::string path,
                            std::string key,
                            std::string name,
                            Targs... exinfo)
            : GeneralException(gex, "path", path, "key", key, "name", name, exinfo...)
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
