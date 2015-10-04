/*! \file
 *
 * \brief Loading and storing of Python modules (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "bpmodule/modulestore/PyModuleLoader.hpp"
#include "bpmodule/modulebase/ModuleBase.hpp"
#include "bpmodule/exception/ModuleLoadException.hpp"
#include "bpmodule/python_helper/Types.hpp"
#include "bpmodule/python_helper/Call.hpp"

using bpmodule::modulebase::ModuleBase;
using bpmodule::exception::ModuleLoadException;

namespace bpmodule {
namespace modulestore {



PyModuleLoader::PyModuleLoader(ModuleStore * mst)
    : BASE(mst)
{
}



PyModuleLoader::~PyModuleLoader()
{
    BASE::DeleteAll();
}



ModuleBase * PyModuleLoader::GeneratorWrapper_(boost::python::object fn,
                                               const std::string & name,
                                               unsigned long id,
                                               ModuleStore & mstore,
                                               ModuleInfo & minfo)
{
    boost::python::object newobj = python_helper::CallPyFunc(fn, name, id, boost::ref(mstore), boost::ref(minfo));
    ModuleBase * ptr = boost::python::extract<ModuleBase *>(newobj); // may throw
    BASE::CopyObject(id, newobj); // strong exception guarantee
    return ptr;
}



void PyModuleLoader::LoadPyModule(const std::string & key,
                                  boost::python::object func,
                                  const boost::python::dict & minfo)
{
    // convert moduleinfo
    ModuleInfo mi(minfo);

    // check if callable
    // 4 arguments - name, id, mstore, minfo
    if(!python_helper::IsCallable(func, 4))
        throw ModuleLoadException("Cannot load module: Object not callable with four arguments",
                                   mi.path, key, mi.name);

    ModuleStore::ModuleGeneratorFunc cfunc = std::bind(&PyModuleLoader::GeneratorWrapper_, this,
                                                       func,
                                                       std::placeholders::_1,
                                                       std::placeholders::_2,
                                                       std::placeholders::_3,
                                                       std::placeholders::_4);

    ModuleStore::ModuleRemoverFunc dfunc = std::bind(&PyModuleLoader::DeleteObject, this, std::placeholders::_1);

    // strong exception guarantee
    BASE::InsertModule(key, cfunc, dfunc, mi);  // conversion constructor
}


} // close namespace modulestore
} // close namespace bpmodule
