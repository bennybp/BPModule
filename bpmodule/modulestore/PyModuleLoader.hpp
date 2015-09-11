/*! \file
 *
 * \brief Loading and storing of Python modules (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_PYMODULELOADER_HPP_
#define _GUARD_PYMODULELOADER_HPP_

#include <boost/python.hpp>

#include "bpmodule/modulestore/ModuleLoaderBase.hpp"


namespace bpmodule {
namespace modulestore {


/*! \brief Loads python modules
 *
 * This class is the ultimate owner of all created
 * python module objects, which it stores via ModuleLoaderBase.
 */
class PyModuleLoader : public ModuleLoaderBase<boost::python::object>
{
    public:

        /*! \copydoc ModuleLoaderBase::ModuleLoaderBase
         */ 
        PyModuleLoader(modulestore::ModuleStore * mst);

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
         * This function will store a copy of the python object for use in creating
         * objects.
         *
         * \throw bpmodule::exception::GeneralException if there is a problem loading
         *        the module (does not exist, etc)
         *
         * \exstrong
         *
         * \param [in] key The key for this module
         * \param [in] func Python object that is callable and which
         *                  returns a pointer to an object derived from ModuleBase
         * \param [in] minfo The module information, including the path and name
         */ 
        void AddPyModule(const std::string & key,
                         boost::python::object func,
                         const boost::python::dict & minfo);

    private:
        //! This object's base class
        typedef ModuleLoaderBase<boost::python::object> BASE;



        /*! \brief Wraps the function pointer in an SO file
         *
         * \exstrong
         *
         * \param [in] fn Creation function object
         * \param [in] key Key of the module to create
         * \param [in] id ID of the new module
         * \param [in] mstore ModuleStore that is in charge of this object
         * \param [in] minfo The information for this module (including options)
         * \return Pointer to a new object derived from ModuleBase
         */
        modulebase::ModuleBase * CreateWrapper_(boost::python::object fn,
                                                const std::string & key,
                                                unsigned long id,
                                                ModuleStore & mstore,
                                                const ModuleInfo & minfo);


        //! \copydoc AddPyModule
        void AddPyModule_(const std::string & key,
                          boost::python::object func,
                          const ModuleInfo & minfo);
};


} // close namespace modulestore
} // close namespace bpmodule


#endif
