/*! \file
 *
 * \brief Storage of cache data (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "bpmodule/datastore/CacheData.hpp"
#include "bpmodule/output/Output.hpp"
#include "bpmodule/python/Convert.hpp"


namespace bpmodule {
namespace datastore {



size_t CacheData::CountKey(const std::string & key) const
{
    return cmap_.count(key);
}


bool CacheData::HasData(const std::string & key,
                        const OptionMap & opt, const KeySet & sigopt) const
{
    auto range = cmap_.equal_range(key);

    for(auto it = range.first; it != range.second; ++it)
        if(opt.CompareSelect(it->second.options, sigopt))
            return true;

    return false;
}



bool CacheData::HasDataPy(const std::string & key,
                          const OptionMap & opt, pybind11::list sigopt) const
{
    return HasData(key, opt, python::ConvertToCpp<KeySet>(sigopt));
}




KeySet CacheData::GetKeys(void) const
{
    KeySet v;

    for(auto & it : cmap_)
        v.insert(it.first);

    return v;
}



size_t CacheData::Size(void) const noexcept
{
    return cmap_.size();
}




pybind11::object CacheData::GetPy(const std::string & key,
                                  const OptionMap & opt, pybind11::list sigopt) const
{
    return Get<pybind11::object>(key, opt,
                                 python::ConvertToCpp<KeySet>(sigopt));
}



void CacheData::SetPy(const std::string & key, pybind11::object value,
                      const OptionMap & opt)
{
    Set(key, value, opt);
}



size_t CacheData::Erase(const std::string & key)
{
    return cmap_.erase(key);
}



void CacheData::Print(std::ostream & os, size_t) const
{
    using namespace bpmodule::output;

    Output(os, "Cache data with %? entries\n", Size());

    for(const auto & it : cmap_)
        Output(os, "  -Key: %-20?  NOptions: %-4?  Type: %?\n", it.first,
                                                                it.second.options.Size(),
                                                                it.second.value->DemangledType());
}


const CacheData::CacheDataEntry & CacheData::GetOrThrow_(const std::string & key,
                                                         const OptionMap & opt,
                                                         const KeySet & sigopt) const
{
    if(cmap_.count(key))
    {
        auto range = cmap_.equal_range(key);

        for(auto it = range.first; it != range.second; ++it)
        {
            if(sigopt.size() == 0 && opt.Compare(it->second.options))
                return it->second;

            if(sigopt.size() && opt.CompareSelect(it->second.options, sigopt))
                return it->second;
        }

        // if you got here, it was not found
        throw exception::DataStoreException("Key with these options not found in CacheData", "key", key);
    }
    else
        throw exception::DataStoreException("Key not found in CacheData", "key", key);
}




void CacheData::Set_(const std::string & key, std::unique_ptr<detail::GenericBase> && value, const OptionMap & opt)
{
    // emplace has strong exception guarantee
    cmap_.emplace(key, CacheDataEntry{opt, std::move(value)});
}






} // close namespace datastore
} // close namespace bpmodule

