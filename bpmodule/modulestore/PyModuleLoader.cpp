/*! \file
 *
 * \brief Loading and storing of Python modules (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include "bpmodule/modulestore/PyModuleLoader.hpp"
#include "bpmodule/modulebase/ModuleBase.hpp"

using bpmodule::modulebase::ModuleBase;

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



ModuleBase * PyModuleLoader::CreateWrapper_(boost::python::object fn,
                                            const std::string & key,
                                            unsigned long id,
                                            ModuleStore & mstore,
                                            const ModuleInfo & minfo)
{
    boost::python::object newobj = fn(key, id, boost::ref(mstore), boost::ref(minfo));
    BASE::CopyObject(id, newobj);
    return boost::python::extract<ModuleBase *>(newobj);
}



void PyModuleLoader::AddPyModule_(const std::string & key,
                                  boost::python::object func, const ModuleInfo & minfo)
{

    ModuleStore::ModuleGeneratorFunc cfunc = std::bind(&PyModuleLoader::CreateWrapper_, this,
                                                       func,
                                                       std::placeholders::_1,
                                                       std::placeholders::_2,
                                                       std::placeholders::_3,
                                                       std::placeholders::_4);

    ModuleStore::ModuleRemoverFunc dfunc = std::bind(&PyModuleLoader::DeleteObject, this, std::placeholders::_1);
    BASE::AddModule(key, cfunc, dfunc, minfo);
}


void PyModuleLoader::AddPyModule(const std::string & key,
                                 boost::python::object func,
                                 const boost::python::dict & minfo)
{
    AddPyModule_(key, func, ModuleInfo(minfo)); // conversion constructor for ModuleInfo
}


} // close namespace modulestore
} // close namespace bpmodule
