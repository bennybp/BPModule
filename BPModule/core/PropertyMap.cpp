#include "BPModule/core/PropertyMap.hpp"


namespace bpmodule {


void PropertyMap::Add_(const std::string & key, PropPlaceholderPtr && value)
{
    
    if(Has(key))
    {
        PropMapEntry & phe = GetOrThrow_(key);
        phe.value = std::move(value);
    }
    else
    {
        PropMapEntry phe{std::move(value)};
        opmap_.insert(PropMapValue{key, std::move(phe) });
    }
}


size_t PropertyMap::Erase(const std::string & key)
{
    return opmap_.erase(key);
}


size_t PropertyMap::Erase_(const std::string & key)
{
    return opmap_.erase(key);
}


void PropertyMap::AddRef(const PropertyMap & pm, const std::string & key)
{
    if(Has(key))
        throw BPModuleException(
                                 "Adding duplicate key",
                                 {
                                     { "Location", "PropertyMap" },
                                     { "Key", key }
                                 }
                               );
    

    // get the shared ptr, etc, from the other property map
    // This should copy the shared_ptr
    PropMapEntry pe = pm.GetOrThrow_(key);

    // add it here
    opmap_.insert(PropMapValue{key, std::move(pe)});    
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

