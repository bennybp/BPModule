/*! \file
 *
 * \brief Loading and storing of Python supermodules (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_MODULEMANAGER__PYMODULELOADER_HPP_
#define BPMODULE_GUARD_MODULEMANAGER__PYMODULELOADER_HPP_

#include <unordered_map>

#include "bpmodule/modulemanager/ModuleLoaderBase.hpp"
#include "bpmodule/modulemanager/ModuleCreationFuncs.hpp"
#include "bpmodule/python/Pybind11.hpp"


namespace bpmodule {
namespace modulemanager {


/*! \brief Loads python supermodules
 *
 * This loader loads supermodules for python supermodule (a python module 
 * directory containing  * a __init__.py file).
 * Module objects and creation functions are cached
 * and deleted/closed at destruction.
 */
class PyModuleLoader : public ModuleLoaderBase
{
    public:

        PyModuleLoader() = default;


        /*! Destructor
         *
         * Deletes all objects and closes all creation functions
         */
        ~PyModuleLoader();


        PyModuleLoader(const PyModuleLoader &)             = delete;
        PyModuleLoader & operator=(const PyModuleLoader &) = delete;
        PyModuleLoader(PyModuleLoader &&)                  = default;
        PyModuleLoader & operator=(PyModuleLoader &&)      = default;


        virtual const ModuleCreationFuncs & LoadSupermodule(const std::string & spath);


    private:
        //! Holds information about a python supermodule
        struct PyModInfo
        {
            pybind11::module mod;           //!< Actual python module object
            ModuleCreationFuncs creators;   //! Creator functions in that supermodule
        };

        //! Stores all the info about already-opened supermodules
        std::unordered_map<std::string, PyModInfo> objmap_;
};

} // close namespace modulemanager
} // close namespace bpmodule


#endif

