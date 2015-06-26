#include "BPModule/core/OptionMap.hpp"
#include "BPModule/core/Output.hpp"

namespace out = bpmodule::output;

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
                  ));
}


void OptionMap::Change(const std::string & key, std::unique_ptr<OptionPlaceholder> && value)
{
    OpMapEntry & opme = GetOrThrow_(key);
    opme.value = std::move(value);
    opme.changed = true;
}



void OptionMap::InitDefault(const std::string & key, std::unique_ptr<OptionPlaceholder> && def, const std::string & help)
{
    opmap_.insert(OpMapValue(key, OpMapEntry{true, std::move(def), help}));
}



OptionMap & OptionMap::operator=(const OptionMap & rhs)
{
    using std::swap;
    OptionMap tmp(rhs);
    swap(*this, tmp);
    return *this;
}



bool OptionMap::Has(const std::string & key) const
{
    return opmap_.count(key);
}



const std::string OptionMap::GetHelp(const std::string & key) const
{
    if(Has(key))
        return opmap_.at(key).help;
    else
    {
        out::Warning("Cannot find entry for key \"%1%\"\n", key);
        return "";
    }
}



std::string OptionMap::GetType(const std::string & key) const
{
    if(Has(key))
        return opmap_.at(key).value->Type();
    else
    {
        out::Warning("Cannot find entry for key \"%1%\"\n", key);
        return "";
    }
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
        throw MapException("OptionMap", key);
}


OptionMap::OpMapEntry & OptionMap::GetOrThrow_(const std::string & key)
{
    if(opmap_.count(key))
        return opmap_.at(key);
    else
        throw MapException("OptionMap", key);
}


} // close namespace bpmodule


