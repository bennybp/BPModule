#include "BPModule/core/OptionMap.hpp"


namespace bpmodule {


OptionMap::OptionMap(const OptionMap & rhs)
{
    // need to Clone new elements
    for(auto & it : rhs.opmap_)
        opmap_.insert(OpMapValue(
                          it.first,
                          OpMapEntry
                            {
                                it.second.changed,
                                std::unique_ptr<OptionPlaceholder>(it.second.value->Clone()),
                                it.second.help
                            }
                                )
                      );
}


OptionMap & OptionMap::operator=(const OptionMap & rhs)
{
    using std::swap;
    OptionMap tmp(rhs);
    swap(*this, tmp);
    return *this;
}



void OptionMap::Change_(const std::string & key, std::unique_ptr<OptionPlaceholder> && value)
{
    OpMapEntry & opme = GetOrThrow_(key);
    opme.value = std::move(value);
    opme.changed = true;
}



void OptionMap::InitDefault_(const std::string & key, std::unique_ptr<OptionPlaceholder> && def, const std::string & help)
{
    opmap_.insert(OpMapValue(key, OpMapEntry{true, std::move(def), help}));
}



bool OptionMap::Has(const std::string & key) const
{
    return opmap_.count(key);
}



std::string OptionMap::GetHelp(const std::string & key) const
{
    return GetOrThrow_(key).help;
}



std::string OptionMap::GetType(const std::string & key) const
{
    return GetOrThrow_(key).value->Type();
}



std::vector<std::string>
OptionMap::GetKeys(void) const
{
    std::vector<std::string> v;
    v.reserve(opmap_.size());
    for(auto & it : opmap_)
        v.push_back(it.first);
    return v;
}



size_t OptionMap::Size(void) const
{
    return opmap_.size();
}



size_t OptionMap::Erase_(const std::string & key)
{
    return opmap_.erase(key);
}



const OptionMap::OpMapEntry & OptionMap::GetOrThrow_(const std::string & key) const
{
    if(opmap_.count(key))
        return opmap_.at(key);
    else
        throw BPModuleException(
                                 "Key not found",
                                 {
                                     { "Location", "OptionMap" },
                                     { "Key", key }
                                 }
                               );
}


OptionMap::OpMapEntry & OptionMap::GetOrThrow_(const std::string & key)
{
    if(opmap_.count(key))
        return opmap_.at(key);
    else
        throw BPModuleException(
                                 "Key not found",
                                 {
                                     { "Location", "OptionMap" },
                                     { "Key", key }
                                 }
                               );
}


} // close namespace bpmodule

