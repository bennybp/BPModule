/*! \file
 *
 * \brief Loading and storing of C/C++ modules (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_MODULEMANAGER__CPPMODULELOADER_HPP_
#define BPMODULE_GUARD_MODULEMANAGER__CPPMODULELOADER_HPP_

#include "bpmodule/modulemanager/ModuleCreationFuncs.hpp"


namespace bpmodule {
namespace modulemanager {


// forward declaration
class ModuleManager;
struct ModuleInfo;


/*! \brief Loads C/C++ modules from an SO file
 */
class CppModuleLoader
{
    public:

        /*! Constructor
         * 
         * \param [in] mm The module manager to load modules into
         */
        CppModuleLoader(ModuleManager * mm);


        /*! Destructor
         *
         * Deletes all objects and closes all SO file handles
         */
        ~CppModuleLoader();

        CppModuleLoader(const CppModuleLoader &)             = delete;
        CppModuleLoader(CppModuleLoader &&)                  = delete;
        CppModuleLoader & operator=(const CppModuleLoader &) = delete;
        CppModuleLoader & operator=(CppModuleLoader &&)      = delete;



        /*! \brief Loads an SO file for a C/C++ module and inserts it into the database
         *
         * The module path of the \p minfo parameter must be set to the full path to the .so file
         *
         * This functions opens the SO file and obtains a pointer
         * to the creation and initialization functions contained in the module. It then inserts it
         * into the ModuleManager associated with this object.
         *
         * If the SO file has already been opened, it will reuse the
         * existing handle.
         *
         * The key in the \minfo parameter must be unique. An exception is thrown if the key already exists
         * in the ModuleManager.
         *
         * \throw bpmodule::exception::ModuleLoadException if there is a problem loading
         *        the module (duplicate key, function doesn't exist, etc) or if minfo
         *        doesn't contain a path
         *
         * \exbasic
         *
         * \param [in] key The key for this module
         * \param [in] minfo The module information, including the path and name
         */
        void LoadSO(const ModuleInfo & minfo);


    private:
        //! The ModuleManager to load modules into
        ModuleManager * mm_;

        //! Holds information about a module so file
        struct SOInfo
        {
            void * handle;                //!< Handle return from dlopen
            ModuleCreationFuncs creators; //!< Creator functions in that module
        };

        //! Stores handles and creation funcs to open SO files
        std::unordered_map<std::string, SOInfo> soinfo_;
};

} // close namespace modulemanager
} // close namespace bpmodule


#endif
