#include "BPModule/core/Output.h"
#include "BPModule/core/PyModuleLoader.h"
#include "BPModule/core/ModuleStore.h"
#include "BPModule/core/ModuleBase.h"

namespace bpmodule {



PyModuleLoader::PyModuleLoader(ModuleStore * mst)
    : mst_(mst)
{
}



PyModuleLoader::~PyModuleLoader()
{
    DeleteAll();
}



ModuleBase * PyModuleLoader::FuncWrapper_(boost::python::object fn, const std::string & key,
                                          unsigned long id,
                                          const OptionMap & options)
{
    boost::python::object newobj = fn(key, id, boost::ref(*mst_), options);
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

    ModuleGeneratorFunc cfunc = std::bind(&PyModuleLoader::FuncWrapper_, this, func, 
                                                        std::placeholders::_1,
                                                        std::placeholders::_2,
                                                        std::placeholders::_3);

    ModuleDeleterFunc dfunc = std::bind(&PyModuleLoader::DeleteWrapper_, this, std::placeholders::_1);
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