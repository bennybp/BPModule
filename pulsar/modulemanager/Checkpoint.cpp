/*! \file
 *
 * \brief Checkpointing of a calcualtion (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "pulsar/modulemanager/Checkpoint.hpp"
#include "pulsar/modulemanager/ModuleManager.hpp"
#include "pulsar/datastore/CacheData.hpp"
#include "pulsar/output/GlobalOutput.hpp"
#include "pulsar/math/EigenImpl.hpp"
#include "pulsar/util/Time.hpp"

#include <bphash/types/vector.hpp>

#include <fstream>
#include <unistd.h> //! \todo removeme

using namespace pulsar::output;
using namespace pulsar::datastore;


namespace {

    /*! \brief Data object stored in the checkpoint file */
    struct CheckPointData
    {
        std::string modulekey;
        std::string cachekey;
        bphash::HashValue hash;
        ByteArray data;
    };
};


namespace pulsar {
namespace modulemanager {



Checkpoint::Checkpoint(const std::string & path)
    : path_(path), cur_modid_(0)
{ }


void Checkpoint::save(const ModuleManager & mm)
{
    print_global_output("Starting checkpoint at %?\n", util::timestamp_string());

    std::lock_guard<std::mutex> l_mm(mm.mutex_);

    std::set<std::string> keys_inuse;
    std::set<std::string> keys_available;

    // get inuse modules
    for(const auto & it : mm.modules_inuse_)
    {
        // get the moduleinfo and make the cache key
        const ModuleInfo & minfo = mm.mtree_.get_by_id(it).minfo;
        std::string cckey = mm.make_cache_key_(minfo);
        keys_inuse.insert(cckey);
    }

    print_global_output("Cache entries:\n");
    for(const auto & it : mm.cachemap_)
    {
        unsigned long modid = 0;

        // assign an id if not found
        if(modid_map_.count(it.first) == 0)
        {
            modid = cur_modid_++;
            modid_map_.emplace(it.first, modid);
        }
        else
            modid = modid_map_.at(it.first);


        bool inuse = keys_inuse.count(it.first);
        print_global_output("    %10?  %4?  %?\n", inuse ? "(in use)" : "", modid, it.first);


        if(!inuse)
            keys_available.insert(it.first);
    }

    for(const auto & key : keys_available)
    {
        print_global_output("++ Checkpointing %?\n", key);
        save_module_cache_(mm.cachemap_.at(key), modid_map_.at(key));
    }

    //sleep(5);

    print_global_output("Checkpoint finished at %?\n", util::timestamp_string());
}


void Checkpoint::save_module_cache_(const CacheData & cd, unsigned long modid)
{
    int serializable = 0;
    for(const auto & it : cd.cmap_)
        if(it.second.value->is_serializable())
            serializable++;

    print_global_output("     > ID %? : %? entries, %? serializable\n", modid, cd.size(), serializable);

    cd.print(get_global_output());

    for(const auto & it : cd.cmap_)
    {
        if(it.second.value->is_serializable())
        {
            ByteArray bar = it.second.value->to_byte_array();

            std::string hashstr = "(not hashable)";

            if(it.second.value->is_hashable())
                hashstr = bphash::hash_to_string(it.second.value->my_hash());

            print_global_output("    + entry: %?  hash: %?  size: %?\n", it.first, hashstr, bar.size());
        }

    }

    print_global_output("\n");

}



} // close namespace modulemanager
} // close namespace pulsar

