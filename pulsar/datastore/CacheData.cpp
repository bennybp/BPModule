#include "pulsar/datastore/CacheData.hpp"

namespace pulsar {
namespace datastore {

size_t CacheData::count(const std::string & key) const
{
    return parent_cmap_->count(make_full_key_(key));
}

std::set<std::string> CacheData::get_keys(void) const
{
    return get_my_keys_();
}

size_t CacheData::size(void) const noexcept
{
    return get_my_keys_().size();
}

size_t CacheData::erase(const std::string & key)
{
    return parent_cmap_->erase(make_full_key_(key));
}

} // close namespace datastore
} // close namespace pulsar

