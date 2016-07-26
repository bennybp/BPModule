/*! \file
 *
 * \brief Checkpointing of a calcualtion (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "pulsar/modulemanager/Checkpoint.hpp"
#include "pulsar/modulemanager/ModuleManager.hpp"
#include "pulsar/datastore/CacheData.hpp"
#include "pulsar/output/GlobalOutput.hpp"
#include "pulsar/util/Time.hpp"
#include <unistd.h> //! \todo removeme

using namespace pulsar::output;
using namespace pulsar::datastore;


namespace pulsar {
namespace modulemanager {


Checkpoint::Checkpoint(const std::string & path)
    : path_(path)
{ }


void Checkpoint::save(const ModuleManager & mm)
{
    print_global_output("Starting checkpoint at %?\n", util::timestamp_string());

    std::lock_guard<std::mutex> l_mm(mm.mutex_);

    std::set<std::string> keys_inuse;
    std::set<std::string> keys_available;

    print_global_output("Modules in use:\n");
    for(const auto & it : mm.modules_inuse_)
    {
        // get the moduleinfo and make the cache key
        const ModuleInfo & minfo = mm.mtree_.get_by_id(it).minfo;

        std::string cckey = mm.make_cache_key_(minfo);
        keys_inuse.insert(cckey);

        print_global_output("    %?   (%? v%?)\n", it, minfo.name, minfo.version);
    }

    print_global_output("Cache entries:\n");
    for(const auto & it : mm.cachemap_)
    {
        bool inuse = keys_inuse.count(it.first);
        print_global_output("    %10?  %?\n", inuse ? "(in use)" : "", it.first);

        if(!inuse)
            keys_available.insert(it.first);
    }

    for(const auto & key : keys_available)
    {
        print_global_output("++ Checkpointing %?\n", key);
        save_module_cache_(mm.cachemap_.at(key));
    }

    sleep(5);

    print_global_output("Checkpoint finished at %?\n", util::timestamp_string());
}


void Checkpoint::save_module_cache_(const CacheData & cd)
{

    int serializable = 0;
    for(const auto & it : cd.cmap_)
        if(it.second.value->is_serializable())
            serializable++;

    print_global_output("     > %? entries, %? serializable\n", cd.size(), serializable);
    cd.print(get_global_output());
    print_global_output("\n");

}



} // close namespace modulemanager
} // close namespace pulsar

