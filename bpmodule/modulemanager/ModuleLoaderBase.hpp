/*! \file
 *
 * \brief Loading and storing of C/C++ modules (base class header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_MODULEMANAGER__MODULELOADERBASE_HPP_
#define BPMODULE_GUARD_MODULEMANAGER__MODULELOADERBASE_HPP_

#include "bpmodule/modulemanager/ModuleCreationFuncs.hpp"


namespace bpmodule {
namespace modulemanager {


/*! \brief A base class for module loaders
 */
class ModuleLoaderBase
{
    public:

        ModuleLoaderBase() = default;
        virtual ~ModuleLoaderBase() = default;

        ModuleLoaderBase(const ModuleLoaderBase &)             = delete;
        ModuleLoaderBase(ModuleLoaderBase &&)                  = delete;
        ModuleLoaderBase & operator=(const ModuleLoaderBase &) = delete;
        ModuleLoaderBase & operator=(ModuleLoaderBase &&)      = delete;


        /*! \brief Load a supermodule module
         *
         * \throw bpmodule::exception::ModuleLoadException if there
         *        is a problem (missing function, etc)
         *
         * \exbasic
         *
         * \param [in] spath The path to the supermodule
         * \return Functions that is used to create modules in that supermodule
         */
        virtual const ModuleCreationFuncs & LoadSupermodule(const std::string & spath) = 0;
};

} // close namespace modulemanager
} // close namespace bpmodule


#endif
