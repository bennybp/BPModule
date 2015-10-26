/*! \file
 *
 * \brief Loading and storing of Python modules (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include <boost/python/extract.hpp>

#include "bpmodule/modulelocator/PyModuleLoader.hpp"
#include "bpmodule/modulebase/ModuleBase.hpp"
#include "bpmodule/exception/ModuleLoadException.hpp"
#include "bpmodule/python_helper/Types.hpp"
#include "bpmodule/python_helper/Call.hpp"

using bpmodule::modulebase::ModuleBase;
using bpmodule::exception::ModuleLoadException;

namespace bpmodule {
namespace modulelocator {



PyModuleLoader::PyModuleLoader(ModuleLocator * mlt)
    : BASE(mlt)
{
}



PyModuleLoader::~PyModuleLoader()
{
    BASE::DeleteAll();
}



ModuleBase * PyModuleLoader::GeneratorWrapper_(boost::python::object fn,
                                               const std::string & name,
                                               unsigned long id,
                                               ModuleLocator & mlocator,
                                               ModuleInfo & minfo)
{
    boost::python::object newobj = python_helper::CallPyFunc<boost::python::object>(fn, name, id, boost::ref(mlocator), boost::ref(minfo));
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
    // 4 arguments - name, id, mlocator, minfo
    if(!python_helper::IsCallable(func, 4))
        throw ModuleLoadException("Cannot load module: Object not callable with four arguments",
                                   mi.path, key, mi.name);

    ModuleLocator::ModuleGeneratorFunc cfunc = std::bind(&PyModuleLoader::GeneratorWrapper_, this,
                                                       func,
                                                       std::placeholders::_1,
                                                       std::placeholders::_2,
                                                       std::placeholders::_3,
                                                       std::placeholders::_4);

    ModuleLocator::ModuleRemoverFunc dfunc = std::bind(&PyModuleLoader::DeleteObject, this, std::placeholders::_1);

    // strong exception guarantee
    BASE::InsertModule(key, cfunc, dfunc, mi);  // conversion constructor
}


} // close namespace modulelocator
} // close namespace bpmodule
