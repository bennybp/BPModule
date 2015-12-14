/*! \file
 *
 * \brief Loading and storing of Python modules (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

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



ModuleBase * PyModuleLoader::GeneratorWrapper_(pybind11::object fn,
                                               const std::string & name,
                                               unsigned long id)
{
    pybind11::object newobj = python_helper::CallPyFunc2<pybind11::object>(fn, name, id);
    ModuleBase * ptr = newobj.cast<ModuleBase *>(); // may throw
    BASE::CopyObject(id, newobj); // strong exception guarantee
    return ptr;
}



void PyModuleLoader::LoadPyModule(const std::string & key,
                                  pybind11::object func,
                                  pybind11::dict minfo)
{
    // convert moduleinfo
    ModuleInfo mi(minfo);

    // check if callable
    // 2 arguments - name, id
    //! \todo nargs
    if(!python_helper::IsCallable2(func))
        throw ModuleLoadException("Cannot load module: Object not callable",
                                   mi.path, key, mi.name);

    ModuleLocator::ModuleGeneratorFunc cfunc = std::bind(&PyModuleLoader::GeneratorWrapper_, this,
                                                       func,
                                                       std::placeholders::_1,
                                                       std::placeholders::_2);

    ModuleLocator::ModuleRemoverFunc dfunc = std::bind(&PyModuleLoader::DeleteObject, this, std::placeholders::_1);

    // strong exception guarantee
    BASE::InsertModule(key, cfunc, dfunc, mi);  // conversion constructor
}


} // close namespace modulelocator
} // close namespace bpmodule
