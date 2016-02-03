/*! \file
 *
 * \brief Loading and storing of C/C++ modules (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_MODULEMANAGER__CPPMODULELOADER_HPP_
#define BPMODULE_GUARD_MODULEMANAGER__CPPMODULELOADER_HPP_

#include "bpmodule/modulemanager/ModuleLoaderBase.hpp"


namespace bpmodule {
namespace modulemanager {


/*! \brief Loads C/C++ modules from an SO file
 */
class CppModuleLoader : public ModuleLoaderBase
{
    public:

        CppModuleLoader() = default;


        /*! Destructor
         *
         * Deletes all objects and closes all SO file handles
         */
        ~CppModuleLoader();

        CppModuleLoader(const CppModuleLoader &)             = delete;
        CppModuleLoader(CppModuleLoader &&)                  = delete;
        CppModuleLoader & operator=(const CppModuleLoader &) = delete;
        CppModuleLoader & operator=(CppModuleLoader &&)      = delete;

        virtual const ModuleCreationFuncs & LoadSupermodule(const std::string & spath);

    private:
        //! Holds information about a module so file
        struct SOInfo
        {
            void * handle;                //!< Handle returned from dlopen
            ModuleCreationFuncs creators; //!< Creator functions in that supermodule
        };

        //! Stores handles and creation funcs to open SO files
        std::unordered_map<std::string, SOInfo> soinfo_;
};

} // close namespace modulemanager
} // close namespace bpmodule


#endif
