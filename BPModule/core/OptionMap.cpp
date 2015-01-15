#include "BPModule/core/OptionMap.h"
#include "BPModule/core/Output.h"

void swap(bpmodule::OptionMap & first, bpmodule::OptionMap & second)
{
    std::swap(first.opmap_, second.opmap_);
} 

namespace bpmodule {


OptionMap::OptionMap(OptionMap && rhs)
{
    swap(*this, rhs);
}

// copy and swap
OptionMap & OptionMap::operator=(OptionMap rhs)
{
    swap(*this, rhs);
    return *this;
}

OptionMap::OptionMap(const OptionMap & rhs)
{
    // need to Clone new elements
    for(auto & it : rhs.opmap_)
        opmap_.insert(OpMapValue(it.first, OpMapEntry({it.second.oph->Clone(), it.second.help})));
}

bool OptionMap::Has(const std::string & key) const
{
    return opmap_.count(key);
}

std::string OptionMap::GetHelp(const std::string & key) const
{
    if(Has(key))
        return opmap_.at(key).help;
    else
    {
        Warning("Cannot find entry for key \"%1%\"", key);
        return "";
    }
}

std::unordered_map<std::string, std::string> OptionMap::GetAllHelp(void) const
{
    std::unordered_map<std::string, std::string> m;
    for(auto & it : opmap_)
        m.insert(std::pair<std::string, std::string>(it.first, it.second.help));
    return m;
}

std::string OptionMap::GetValueStr(const std::string & key) const
{
    if(Has(key))
        return opmap_.at(key).oph->ToString();
    else
    {
        Warning("Cannot find entry for key \"%1%\"", key);
        return "";
    }
}

std::unordered_map<std::string, std::string> OptionMap::GetAllValueStr(void) const
{
    std::unordered_map<std::string, std::string> m;
    for(auto & it : opmap_)
        m.insert(std::pair<std::string, std::string>(it.first, it.second.oph->ToString()));
    return m;
}


size_t OptionMap::Size(void) const
{
    return opmap_.size();
}


size_t OptionMap::Erase(const std::string & key)
{
    // delete pointer if necessary
    if(opmap_.count(key))
        delete opmap_.at(key).oph; // delete the ptr

    return opmap_.erase(key);       
}


const OptionMap::OpMapEntry & OptionMap::GetOrThrow(const std::string & key) const
{
    if(opmap_.count(key))
        return opmap_.at(key);
    else
        throw MapException("OptionMap", key);
}

} // close namespace bpmodule


