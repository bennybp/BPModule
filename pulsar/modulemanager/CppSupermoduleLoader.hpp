/*! \file
 *
 * \brief Loading and storing of C/C++ supermodules (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_MODULEMANAGER__CPPSUPERMODULELOADER_HPP_
#define BPMODULE_GUARD_MODULEMANAGER__CPPSUPERMODULELOADER_HPP_

#include <map>

#include "bpmodule/modulemanager/SupermoduleLoaderBase.hpp"
#include "bpmodule/modulemanager/ModuleCreationFuncs.hpp"


namespace bpmodule {
namespace modulemanager {


/*! \brief Loader for C++ supermodules
 *
 * This loader loads supermodules for C++ modules from a
 * shared object module (.so) file. Handles and creation functions
 * are cached and closed at destruction.
 */
class CppSupermoduleLoader : public SupermoduleLoaderBase
{
    public:

        CppSupermoduleLoader() = default;


        /*! Destructor
         *
         * Deletes all objects and closes all SO file handles
         */
        virtual ~CppSupermoduleLoader();


        CppSupermoduleLoader(const CppSupermoduleLoader &)             = delete;
        CppSupermoduleLoader & operator=(const CppSupermoduleLoader &) = delete;
        CppSupermoduleLoader(CppSupermoduleLoader &&)                  = default;
        CppSupermoduleLoader & operator=(CppSupermoduleLoader &&)      = default;


        virtual const ModuleCreationFuncs & LoadSupermodule(const std::string & spath);


    private:
        //! Holds information about a supermodule so file
        struct SOInfo
        {
            void * handle;                //!< Handle returned from dlopen
            ModuleCreationFuncs creators; //!< Creator functions in that supermodule
        };

        //! Stores handles and creation funcs to open SO files
        std::map<std::string, SOInfo> soinfo_;
};

} // close namespace modulemanager
} // close namespace bpmodule


#endif
