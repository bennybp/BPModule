#include "BPModule/core/Output.hpp"
#include "BPModule/core/PyModuleLoader.hpp"
#include "BPModule/core/ModuleStore.hpp"
#include "BPModule/core/ModuleBase.hpp"

namespace bpmodule {



PyModuleLoader::PyModuleLoader(ModuleStore * mst)
    : mst_(mst)
{
}



PyModuleLoader::~PyModuleLoader()
{
    DeleteAll();
}



ModuleBase * PyModuleLoader::CreateWrapper_(boost::python::object fn, const std::string & key,
                                            unsigned long id,
                                            const ModuleInfo & minfo)
{
    boost::python::object newobj = fn(key, id, boost::ref(*mst_), boost::ref(minfo));
    objects_[id] = newobj;
    return boost::python::extract<ModuleBase *>(newobj);
}



void PyModuleLoader::DeleteWrapper_(unsigned long id)
{
    DeleteObject_(id);
}



void PyModuleLoader::AddPyModule(const std::string & key,
                                 boost::python::object func, const ModuleInfo & minfo)
{

    ModuleStore::ModuleGeneratorFunc cfunc = std::bind(&PyModuleLoader::CreateWrapper_, this, func,
                                                       std::placeholders::_1,
                                                       std::placeholders::_2,
                                                       std::placeholders::_3);

    ModuleStore::ModuleDeleterFunc dfunc = std::bind(&PyModuleLoader::DeleteWrapper_, this, std::placeholders::_1);
    mst_->AddCreateFunc(key, cfunc, dfunc, minfo);
}



void PyModuleLoader::DeleteObject_(unsigned long id)
{
    objects_.erase(id);
}



void PyModuleLoader::DeleteAll(void)
{
    objects_.clear();
}


namespace export_python {

// wraps PyModuleLoader::AddPyModule so that it can take a dict for the ModuleInfo
void Wrap_PyModuleLoader_AddPyModule(PyModuleLoader * ml,
                                     const std::string & key, bpy::object func,
                                     const bpy::dict & minfo)
{
    // dictionary is converted to ModuleInfo via constructor
    return ml->AddPyModule(key, func, minfo);
}

} // close namespace export_python


} // close namespace bpmodule
