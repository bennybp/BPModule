/*! \file
 *
 * \brief Loading and storing of C modules (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_CMODULELOADER_HPP_
#define _GUARD_CMODULELOADER_HPP_

#include "bpmodule/python/Pybind11.hpp"
#include "bpmodule/modulelocator/CreatorFunctions.hpp"


namespace bpmodule {
namespace modulelocator {


// forward declaration
class ModuleLocator;



/*! \brief Loads C/C++ modules
 */
class CModuleLoader
{
    public:

        /*! \copydoc ModuleLoaderBase::ModuleLoaderBase
         */
        CModuleLoader(ModuleLocator * mlt);


        /*! Destructor
         *
         * Deletes all objects and closes all SO file handles
         */
        ~CModuleLoader();

        CModuleLoader(const CModuleLoader & rhs)             = delete;
        CModuleLoader(CModuleLoader && rhs)                  = delete;
        CModuleLoader & operator=(const CModuleLoader & rhs) = delete;
        CModuleLoader & operator=(CModuleLoader && rhs)      = delete;



        /*! \brief Loads an SO file for a C/C++ module and inserts it into the database
         *
         * The module path and name must be stored in the module info
         *
         * This functions opens the SO file (via dlopen) and obtains a pointer
         * to the creation function contained in the module, and inserts it
         * into the database. If the SO file has already been opened, it will reuse the
         * existing handle.
         *
         * The key must be unique. An exception is thrown if the key already exists.
         *
         * \throw bpmodule::exception::ModuleLoadException if there is a problem loading
         *        the module (duplicate key, function doesn't exist, etc)
         *
         * \throw bpmodule::exception::ModuleLocatorException if there is a problem converting
         *        the module info.
         *
         * \exstrong
         *
         * \param [in] key The key for this module
         * \param [in] minfo The module information, including the path and name
         */
        void LoadSO(const std::string & key, pybind11::dict minfo);


    private:
        ModuleLocator * mlt_;


        //! Stores handles to to open SO files
        std::unordered_map<std::string, void *> handles_;


        //! Stores all the creators in an SO file
        std::unordered_map<std::string, CreatorFunctions> creators_;

};

} // close namespace modulelocator
} // close namespace bpmodule


#endif
