#include "BPModule/core/OptionMap.h"
#include "BPModule/core/Output.h"

namespace bpmodule {


OptionMap::OptionMap(const OptionMap & rhs)
{
    // need to Clone new elements
    for(auto & it : rhs.opmap_)
        opmap_.insert(OpMapValue(it.first, OpMapEntry({it.second.oph->Clone(), it.second.help})));
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

std::string
OptionMap::GetHelp(const std::string & key) const
{
    if(Has(key))
        return opmap_.at(key).help;
    else
    {
        Warning("Cannot find entry for key \"%1%\"\n", key);
        return "";
    }
}

std::map<std::string, std::string>
OptionMap::GetAllHelp(void) const
{
    std::map<std::string, std::string> m;
    for(auto & it : opmap_)
        m.insert(std::pair<std::string, std::string>(it.first, it.second.help));
    return m;
}


std::string OptionMap::GetType(const std::string & key) const
{
    if(Has(key))
        return opmap_.at(key).oph->Type();
    else
    {
        Warning("Cannot find entry for key \"%1%\"\n", key);
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


