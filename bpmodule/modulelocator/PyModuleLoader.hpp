/*! \file
 *
 * \brief Loading and storing of Python modules (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_PYMODULELOADER_HPP_
#define _GUARD_PYMODULELOADER_HPP_

#include "bpmodule/modulelocator/ModuleLoaderBase.hpp"
#include "bpmodule/python_helper/Pybind11.hpp"

namespace bpmodule {
namespace modulelocator {


/*! \brief Loads python modules
 *
 * This class is the ultimate owner of all created
 * python module objects, which it stores via ModuleLoaderBase.
 */
class PyModuleLoader : public ModuleLoaderBase<pybind11::object>
{
    public:

        /*! \copydoc ModuleLoaderBase::ModuleLoaderBase
         */
        PyModuleLoader(modulelocator::ModuleLocator * mlt);


        /*! Destructor
         *
         * Deletes all objects
         */
        ~PyModuleLoader();

        PyModuleLoader(const PyModuleLoader & rhs)             = delete;
        PyModuleLoader(PyModuleLoader && rhs)                  = delete;
        PyModuleLoader & operator=(const PyModuleLoader & rhs) = delete;
        PyModuleLoader & operator=(PyModuleLoader && rhs)      = delete;


        /*! \brief Loads a python module
         *
         * This function will wrap the python object for use in creating
         * objects, and store it in the database.
         *
         * The key must be unique. An exception is thrown if the key already exists.
         *
         * \throw bpmodule::exception::ModuleLoadException if there is a problem loading
         *        the module (duplicate key, etc)
         *
         * \throw bpmodule::exception::ModuleLocatorException if there is a problem converting
         *        the module info.
         *
         * \exstrong
         *
         * \param [in] key The key for this module
         * \param [in] func Python object that is callable and which
         *                  returns a pointer to an object derived from ModuleBase
         * \param [in] minfo The module information, including the path and name
         */
        void LoadPyModule(const std::string & key,
                          pybind11::object func,
                          pybind11::dict minfo);


    private:
        //! This object's base class
        typedef ModuleLoaderBase<pybind11::object> BASE;



        /*! \brief Wraps a python function object that creates module objects
         *
         * \exstrong
         *
         * \param [in] fn Generator function object
         * \param [in] name Name of the module to create
         * \param [in] id Unique ID for the created module
         * \return Pointer to a new object derived from ModuleBase
         */
        modulebase::ModuleBase * GeneratorWrapper_(pybind11::object fn,
                                                   const std::string & name,
                                                   unsigned long id);

};


} // close namespace modulelocator
} // close namespace bpmodule


#endif
