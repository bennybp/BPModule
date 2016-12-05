/*! \file
 *
 * \brief Loading and storing of Python supermodules (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_MODULEMANAGER__PYSUPERMODULELOADER_HPP_
#define PULSAR_GUARD_MODULEMANAGER__PYSUPERMODULELOADER_HPP_

#include <unordered_map>

#include "pulsar/modulemanager/SupermoduleLoaderBase.hpp"
#include "pulsar/modulemanager/ModuleCreationFuncs.hpp"
#include "pulsar/util/Pybind11.hpp"


namespace pulsar{

/*! \brief Loads python supermodules
 *
 * This loader loads supermodules for python supermodule (a python module
 * directory containing  * a __init__.py file).
 * Module objects and creation functions are cached
 * and deleted/closed at destruction.
 */
class PySupermoduleLoader : public SupermoduleLoaderBase
{
    public:

        PySupermoduleLoader() = default;


        /*! Destructor
         *
         * Deletes all objects and closes all creation functions
         */
        ~PySupermoduleLoader();


        PySupermoduleLoader(const PySupermoduleLoader &)             = delete;
        PySupermoduleLoader & operator=(const PySupermoduleLoader &) = delete;
        PySupermoduleLoader(PySupermoduleLoader &&)                  = default;
        PySupermoduleLoader & operator=(PySupermoduleLoader &&)      = default;


        virtual const ModuleCreationFuncs & load_supermodule(const std::string & spath);


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

} // close namespace pulsar


#endif

