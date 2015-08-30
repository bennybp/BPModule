#include <boost/python.hpp>

#include "bpmodule/modulestore/CModuleLoader.hpp"
#include "bpmodule/modulestore/ModuleInfo.hpp"


namespace bpmodule {
namespace modulestore {
namespace export_python {


// wraps CModuleLoader::LoadSO so that it can take a dict for the ModuleInfo
void Wrap_CModuleLoader_LoadSO(CModuleLoader * ml, const std::string & key, const boost::python::dict & minfo)
{
    // dictionary is converted to ModuleInfo via constructor
    return ml->LoadSO(key, minfo);
}



} // close namespace export_python
} // close namespace modulestore
} // close namespace bpmodule
