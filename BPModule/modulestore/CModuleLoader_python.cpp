#include <boost/python.hpp>

#include "BPModule/modulestore/CModuleLoader.hpp"
#include "BPModule/modulestore/ModuleInfo.hpp"

namespace bpy = boost::python;


namespace bpmodule {
namespace modulestore {
namespace export_python {


// wraps CModuleLoader::LoadSO so that it can take a dict for the ModuleInfo
void Wrap_CModuleLoader_LoadSO(CModuleLoader * ml, const std::string & key, const bpy::dict & minfo)
{
    // dictionary is converted to ModuleInfo via constructor
    return ml->LoadSO(key, minfo);
}



} // close namespace export_python
} // close namespace modulestore
} // close namespace bpmodule
