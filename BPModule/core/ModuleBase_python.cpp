#include "BPModule/core/ModuleBase.hpp"
#include "BPModule/core/ModuleStore.hpp"

#include <boost/python.hpp>

namespace bpy = boost::python;

namespace bpmodule {

template<>
bpy::object ModuleBase::GetTrait<>(const std::string & key) const
{
    return traits_.Get<bpy::object>(key);
}

template<>
bpy::object ModuleBase::GetOption<>(const std::string & key) const
{
    return options_.Get<bpy::object>(key);
}

template<>
void ModuleBase::ChangeTrait<>(const std::string & key, const bpy::object & value)
{
    traits_.Change(key, value);
}

template<>
void ModuleBase::ChangeOption<>(const std::string & key, const bpy::object & value)
{
    options_.Change(key, value);
}



} // close namespace bpmodule
