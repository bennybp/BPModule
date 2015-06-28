#include "BPModule/core/ModuleBase.hpp"
#include "BPModule/core/ModuleStore.hpp"

#include <boost/python.hpp>

namespace bpy = boost::python;

namespace bpmodule {

template<>
bpy::object ModuleBase::GetOption<>(const std::string & key) const
{
    return options_.Get<bpy::object>(key);
}

} // close namespace bpmodule
