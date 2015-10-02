/*! \file
 *
 * \brief An exception for module creation errors
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_MODULECREATEEXCEPTION_HPP_
#define _GUARD_MODULECREATEEXCEPTION_HPP_

#include "bpmodule/exception/GeneralException.hpp"

namespace bpmodule {
namespace exception {


/*! \brief An exception thrown when a module object can not be created
 */
class ModuleCreateException : public GeneralException
{
    public:
        /*! \brief Constructor
         *
         * \param [in] what Brief description of the error
         * \param [in] path Path to the module that could't be created
         * \param [in] key  Key of the module that couldn't be created
         * \param [in] name Name of the module that couldn't be created
         * \param [in] exinfo Additional information. Must be an even number of strings
         */
        ModuleCreateException(std::string what,
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
         * \param [in] path Path to the module that could't be created
         * \param [in] key  Key of the module that couldn't be created
         * \param [in] name Name of the module that couldn't be created
         * \param [in] exinfo Additional information. Must be an even number of strings
         */
        template<typename... Targs>
        ModuleCreateException(const GeneralException & gex,
                              std::string path,
                              std::string key,
                              std::string name,
                              Targs... exinfo)
            : GeneralException(gex, "path", path, "key", key, "name", name, exinfo...)
        { }


        ModuleCreateException()                                              = delete;
        ModuleCreateException(const ModuleCreateException & rhs)             = default;
        ModuleCreateException(ModuleCreateException && rhs)                  = default;
        ModuleCreateException & operator=(const ModuleCreateException & rhs) = default;
        ModuleCreateException & operator=(ModuleCreateException && rhs)      = default;
        virtual ~ModuleCreateException() = default;


        //! Return the path to the module that couldn't be created
        const char * Path(void) const noexcept    { return GeneralException::GetField("path"); }

        //! Return the key of the module that couldn't be created
        const char * Key(void) const noexcept     { return GeneralException::GetField("key");  }

        //! Return the name of the module that couldn't be created
        const char * Name(void) const noexcept    { return GeneralException::GetField("name"); }
};



}// close namespace exception
}// close namespace bpmodule



#endif
