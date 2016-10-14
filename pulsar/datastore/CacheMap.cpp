#include "pulsar/datastore/CacheMap.hpp"
#include "pulsar/output/Output.hpp"


namespace pulsar {
namespace datastore {

CacheMap::CacheMap(void)
        : sync_tag_(-1)
{ }


std::set<std::string> CacheMap::get_keys(void) const
{
    std::set<std::string> v;

    std::lock_guard<std::mutex> l(mutex_);
    for(const auto & it : cmap_)
        v.insert(it.first);

    return v;
}


size_t CacheMap::size(void) const noexcept
{
    std::lock_guard<std::mutex> l(mutex_);
    return cmap_.size();
}

size_t CacheMap::erase(const std::string & key)
{
    std::lock_guard<std::mutex> l(mutex_);
    return cmap_.erase(key);
}

void CacheMap::clear(void)
{
    std::lock_guard<std::mutex> l(mutex_);
    cmap_.clear();
}


void CacheMap::print(std::ostream & os) const
{
    using namespace pulsar::output;

    // We lock the mutex here. Therefore, we cannot call CacheMap::size(), etc.
    std::lock_guard<std::mutex> l(mutex_);

    print_output(os, "Cache data with %? entries\n", cmap_.size());

    for(const auto & it : cmap_)
        print_output(os, "  -Key: %-20?  Serializable: %?  Type: %?\n", it.first,
                     it.second.value->is_serializable() ? "Yes" : "No",
                     it.second.value->demangled_type());
}

} // close namespace datastore
} // close namespace pulsar
