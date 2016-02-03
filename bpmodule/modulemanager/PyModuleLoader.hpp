/*! \file
 *
 * \brief Loading and storing of Python modules (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_MODULEMANAGER__PYMODULELOADER_HPP_
#define BPMODULE_GUARD_MODULEMANAGER__PYMODULELOADER_HPP_

#include "bpmodule/modulemanager/ModuleLoaderBase.hpp"


namespace bpmodule {
namespace modulemanager {


/*! \brief Loads C/C++ modules from an SO file
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
        PyModuleLoader(PyModuleLoader &&)                  = delete;
        PyModuleLoader & operator=(const PyModuleLoader &) = delete;
        PyModuleLoader & operator=(PyModuleLoader &&)      = delete;


        virtual const ModuleCreationFuncs & LoadSupermodule(const std::string & spath);


    private:
        //! Holds information about a python module
        struct PyModInfo
        {
            pybind11::module mod;           //!< Actual python module object
            ModuleCreationFuncs creators;   //! Creator functions in that module
        };

        //! Stores all the info about already-opened modules
        std::map<std::string, PyModInfo> objmap_;
};

} // close namespace modulemanager
} // close namespace bpmodule


#endif

