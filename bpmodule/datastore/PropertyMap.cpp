/*! \file
 *
 * \brief Storage of generic data via (key, value) pair (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/datastore/PropertyMap.hpp"


namespace bpmodule {
namespace datastore {


void PropertyMap::Set_(const std::string & key, PropPlaceholderPtr && value)
{
    if(opmap_.count(key))
    {
        PropMapEntry & phe = GetOrThrow_(key);
        phe.value = std::move(value);
    }
    else
    {
        PropMapEntry phe{std::move(value)};

        // emplace has strong exception guarantee
        opmap_.emplace(key, std::move(phe));
    }
}



size_t PropertyMap::Erase(const std::string & key)
{
    return opmap_.erase(key);
}



void PropertyMap::SetRef(const PropertyMap & pm, const std::string & key, const std::string & newkey)
{
    // get the shared ptr, etc, from the other property map
    // This should copy the shared_ptr
    PropMapEntry pe = pm.GetOrThrow_(key);

    // add it here
    // emplaceinsert has strong exception guarantee
    opmap_.emplace(newkey, std::move(pe));    
}



void PropertyMap::SetRef(const PropertyMap & pm, const std::string & key)
{
    //Setref with the same key
    SetRef(pm, key, key);
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



size_t PropertyMap::Size(void) const noexcept
{
    return opmap_.size();
}



const PropertyMap::PropMapEntry & PropertyMap::GetOrThrow_(const std::string & key) const
{
    if(opmap_.count(key))
        return opmap_.at(key);
    else
        throw exception::MapException("Key not found", "PropertyMap", key); 
}



PropertyMap::PropMapEntry & PropertyMap::GetOrThrow_(const std::string & key)
{
    if(opmap_.count(key))
        return opmap_.at(key);
    else
        throw exception::MapException("Key not found", "PropertyMap", key); 
}



} // close namespace datastore
} // close namespace bpmodule

