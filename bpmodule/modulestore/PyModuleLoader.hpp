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

class PyModuleLoader : public ModuleLoaderBase<boost::python::object>
{
    public:

        PyModuleLoader(modulestore::ModuleStore * mst);
        ~PyModuleLoader();

        PyModuleLoader(const PyModuleLoader & rhs)             = delete;
        PyModuleLoader(PyModuleLoader && rhs)                  = delete;
        PyModuleLoader & operator=(const PyModuleLoader & rhs) = delete;
        PyModuleLoader & operator=(PyModuleLoader && rhs)      = delete;


        void AddPyModule(const std::string & key,
                         boost::python::object func,
                         const boost::python::dict & minfo);

    private:
        typedef ModuleLoaderBase<boost::python::object> BASE;



        modulebase::ModuleBase * CreateWrapper_(boost::python::object fn,
                                                const std::string & key,
                                                unsigned long id,
                                                ModuleStore & mstore,
                                                const ModuleInfo & minfo);

        void AddPyModule_(const std::string & key,
                          boost::python::object func,
                          const ModuleInfo & minfo);
};


} // close namespace modulestore
} // close namespace bpmodule


#endif
