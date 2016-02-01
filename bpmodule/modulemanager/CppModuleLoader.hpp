/*! \file
 *
 * \brief Loading and storing of C modules (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_MODULEMANAGER__CPPMODULELOADER_HPP_
#define BPMODULE_GUARD_MODULEMANAGER__CPPMODULELOADER_HPP_

#include "bpmodule/python/Pybind11.hpp"
#include "bpmodule/modulemanager/ModuleCreationFuncs.hpp"


namespace bpmodule {
namespace modulemanager {


// forward declaration
class ModuleManager;
struct ModuleInfo;


/*! \brief Loads C/C++ modules
 */
class CppModuleLoader
{
    public:

        /*! \copydoc ModuleLoaderBase::ModuleLoaderBase
         */
        CppModuleLoader(ModuleManager * mlt);


        /*! Destructor
         *
         * Deletes all objects and closes all SO file handles
         */
        ~CppModuleLoader();

        CppModuleLoader(const CppModuleLoader & rhs)             = delete;
        CppModuleLoader(CppModuleLoader && rhs)                  = delete;
        CppModuleLoader & operator=(const CppModuleLoader & rhs) = delete;
        CppModuleLoader & operator=(CppModuleLoader && rhs)      = delete;



        /*! \brief Loads an SO file for a C/C++ module and inserts it into the database
         *
         * The module path and name must be stored in the module info
         *
         * This functions opens the SO file (via dlopen) and obtains a pointer
         * to the creation function contained in the module, and inserts it
         * into the database. If the SO file has already been opened, it will reuse the
         * existing handle.
         *
         * The key in the \minfo parameter must be unique. An exception is thrown if the key already exists.
         *
         * \throw bpmodule::exception::ModuleLoadException if there is a problem loading
         *        the module (duplicate key, function doesn't exist, etc)
         *
         * \throw bpmodule::exception::ModuleManagerException if there is a problem converting
         *        the module info.
         *
         * \exstrong
         *
         * \param [in] key The key for this module
         * \param [in] minfo The module information, including the path and name
         */
        void LoadSO(const ModuleInfo & minfo);


    private:
        ModuleManager * mlt_;


        //! Stores handles to to open SO files
        std::unordered_map<std::string, void *> handles_;


        //! Stores all the creators in an SO file
        std::unordered_map<std::string, ModuleCreationFuncs> creators_;

};

} // close namespace modulemanager
} // close namespace bpmodule


#endif
