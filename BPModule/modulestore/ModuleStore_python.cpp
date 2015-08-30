#include <boost/python.hpp>

#include "BPModule/modulestore/ModuleStore.hpp"

namespace bpy = boost::python;


namespace bpmodule {
namespace modulestore {
namespace export_python {


void Wrap_ModuleStore_SetOptions(ModuleStore * mst, const std::string & key, bpy::list & opt)
{
    // OptionMap has a conversion constructor
    mst->SetOptions(key, opt);
}


} // close namespace export_python
} // close namespace modulestore
} // close namespace bpmodule

