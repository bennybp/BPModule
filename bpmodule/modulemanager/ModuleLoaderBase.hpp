/*! \file
 *
 * \brief Base class for loading and storing of modules
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_MODULEMANAGER__MODULELOADERBASE_HPP_
#define BPMODULE_GUARD_MODULEMANAGER__MODULELOADERBASE_HPP_


#include <string>


namespace bpmodule {
namespace modulemanager {


// forward declarations
class ModuleCreationFuncs;


/*! \brief A base class for module loaders
 *
 * Classes that derive from this implement loading (and unloading)
 * of supermodules for a given module type.
 *
 * Unloading should be orderly and happen at destruction. 
 * At destruction, it can be assumed that no modules from within
 * any loaded supermodule are active and therefore handles, etc, 
 * can be safely closed.
 */
class ModuleLoaderBase
{
    public:

        ModuleLoaderBase() = default;
        virtual ~ModuleLoaderBase() = default;

        ModuleLoaderBase(const ModuleLoaderBase &)             = delete;
        ModuleLoaderBase & operator=(const ModuleLoaderBase &) = delete;
        ModuleLoaderBase(ModuleLoaderBase &&)                  = default;
        ModuleLoaderBase & operator=(ModuleLoaderBase &&)      = default;


        /*! \brief Load a supermodule
         *
         * \throw bpmodule::exception::ModuleLoadException if there
         *        is a problem (missing function, etc)
         *
         * \param [in] spath The path to the supermodule to be loaded
         * \return Functions that are used to create modules in that supermodule
         */
        virtual const ModuleCreationFuncs & LoadSupermodule(const std::string & spath) = 0;
};

} // close namespace modulemanager
} // close namespace bpmodule


#endif
