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


// forward declaration
struct ModuleInfo;


/*! \brief Loads C/C++ modules from an SO file
 */
class ModuleLoaderBase
{
    public:

        /*! Constructor
         * 
         * \param [in] mm The module manager to load modules into
         */
        ModuleLoaderBase() = default;


        virtual ~ModuleLoaderBase() = default;

        ModuleLoaderBase(const ModuleLoaderBase &)             = delete;
        ModuleLoaderBase(ModuleLoaderBase &&)                  = delete;
        ModuleLoaderBase & operator=(const ModuleLoaderBase &) = delete;
        ModuleLoaderBase & operator=(ModuleLoaderBase &&)      = delete;


        virtual ModuleCreationFuncs::Func LoadModule(const ModuleInfo & minfo) = 0;
};

} // close namespace modulemanager
} // close namespace bpmodule


#endif
