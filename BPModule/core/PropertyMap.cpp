#include "BPModule/core/PropertyMap.hpp"


namespace bpmodule {


PropertyMap::PropertyMap(const PropertyMap & rhs)
{
    // need to Clone new elements
    for(auto & it : rhs.opmap_)
        opmap_.insert(PropMapValue(
                          it.first,
                          PropMapEntry
                            {
                                it.second.changed,
                                std::unique_ptr<PropPlaceholder>(it.second.value->Clone()),
                            }
                                )
                      );
}


PropertyMap & PropertyMap::operator=(const PropertyMap & rhs)
{
    using std::swap;
    PropertyMap tmp(rhs);
    swap(*this, tmp);
    return *this;
}



void PropertyMap::Change_(const std::string & key, std::unique_ptr<PropPlaceholder> && value)
{
    PropMapEntry & opme = GetOrThrow_(key);
    opme.value = std::move(value);
    opme.changed = true;
}



void PropertyMap::InitDefault_(const std::string & key, std::unique_ptr<PropPlaceholder> && def)
{
    opmap_.insert(PropMapValue(key, PropMapEntry{true, std::move(def)}));
}



bool PropertyMap::Has(const std::string & key) const
{
    return opmap_.count(key);
}



std::string PropertyMap::GetType(const std::string & key) const
{
    return GetOrThrow_(key).value->Type();
}



std::vector<std::string>
PropertyMap::GetKeys(void) const
{
    std::vector<std::string> v;
    v.reserve(opmap_.size());
    for(auto & it : opmap_)
        v.push_back(it.first);
    return v;
}



size_t PropertyMap::Size(void) const
{
    return opmap_.size();
}



size_t PropertyMap::Erase_(const std::string & key)
{
    return opmap_.erase(key);
}



const PropertyMap::PropMapEntry & PropertyMap::GetOrThrow_(const std::string & key) const
{
    if(opmap_.count(key))
        return opmap_.at(key);
    else
        throw BPModuleException(
                                 "Key not found",
                                 {
                                     { "Location", "PropertyMap" },
                                     { "Key", key }
                                 }
                               );
}


PropertyMap::PropMapEntry & PropertyMap::GetOrThrow_(const std::string & key)
{
    if(opmap_.count(key))
        return opmap_.at(key);
    else
        throw BPModuleException(
                                 "Key not found",
                                 {
                                     { "Location", "PropertyMap" },
                                     { "Key", key }
                                 }
                               );
}


} // close namespace bpmodule

