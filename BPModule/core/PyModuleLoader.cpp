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
    boost::python::object newobj = fn(key, id, boost::ref(*mst_), minfo);
    objects_[id] = newobj;
    return boost::python::extract<ModuleBase *>(newobj);
}



void PyModuleLoader::DeleteWrapper_(unsigned long id)
{
    DeleteObject_(id);
}



bool PyModuleLoader::AddPyModule(const std::string & key,
                                 boost::python::object func, const ModuleInfo & minfo)
{

    ModuleStore::ModuleGeneratorFunc cfunc = std::bind(&PyModuleLoader::CreateWrapper_, this, func,
                                                       std::placeholders::_1,
                                                       std::placeholders::_2,
                                                       std::placeholders::_3);

    ModuleStore::ModuleDeleterFunc dfunc = std::bind(&PyModuleLoader::DeleteWrapper_, this, std::placeholders::_1);
    return mst_->AddCreateFunc(key, cfunc, dfunc, minfo);
}



void PyModuleLoader::DeleteObject_(unsigned long id)
{
    objects_.erase(id);
}



void PyModuleLoader::DeleteAll(void)
{
    objects_.clear();
}



} // close namespace bpmodule
