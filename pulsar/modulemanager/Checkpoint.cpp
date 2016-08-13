/*! \file
 *
 * \brief  checkpointing of a calculation (source)
 */

#include "pulsar/modulemanager/Checkpoint.hpp"
#include "pulsar/modulemanager/CheckpointIO.hpp"
#include "pulsar/modulemanager/ModuleManager.hpp"
#include "pulsar/output/GlobalOutput.hpp"
#include "pulsar/util/Serialization.hpp"

using namespace pulsar::exception;
using namespace pulsar::datastore;
using namespace pulsar::output;


namespace {

struct CacheEntryMetadata
{
    std::string modkey;
    std::string cachekey;
    bphash::HashValue hash;  //!< Hash of original data, NOT serialized data
    size_t size;             //!< Size of serialized data
    unsigned int policy;     //!< Storage policy

    template<typename Archive>
    void serialize(Archive & ar)
    {
        ar(modkey, cachekey, hash, size, policy);
    }
};


}



namespace pulsar {
namespace modulemanager {


Checkpoint::Checkpoint(const std::shared_ptr<CheckpointIO> & backend_local,
                       const std::shared_ptr<CheckpointIO> & backend_global)
    : backend_local_(backend_local),
      backend_global_(backend_global)
{ }


std::string Checkpoint::make_cache_key_(const std::string & modkey,
                                        const std::string & datakey)
{
    return std::string("CHKPT_CACHE__") + modkey + "__" + datakey;
}


std::map<std::string, std::vector<std::string>>
Checkpoint::form_local_cache_save_list_(const ModuleManager & mm,
                                        CheckpointIO & backend)
{ 
    // modulemanager should be locked already!
    // print out some info and get what we should be checkpointing
    print_global_output("Cache entries in the module manager:\n");

    std::map<std::string, std::vector<std::string>> to_save;

	for(const auto & cd : mm.cachemap_)
    {
		print_global_output("    %?\n", cd.first);

        for(const auto & data : cd.second.cmap_)
        {
            const auto & gb = *(data.second.value);

            bool save = false;
            char stat = ' ';

            if(data.second.policy & CacheData::CheckpointLocal && gb.is_serializable())
            {
                std::string fullkey = make_cache_key_(cd.first, data.first);

                // does the data already exist?
                if(backend.count(fullkey))
                {
                    // if it's hashable, read the metadata and compare the hashes
                    if(gb.is_hashable())
                    {
                        ByteArray ba_meta = backend.read_metadata(fullkey);
                        auto meta = util::from_byte_array<CacheEntryMetadata>(ba_meta);

                        auto newhash = gb.my_hash();
                        if(newhash != meta.hash)
                        {
                            save = true;
                            stat = 'O';
                        }
                        else
                        {
                            save = false;
                            stat = 'E';
                        }

                    }
                    else
                    {
                        // always err on saving it, even though it may already
                        // be written (we can't hash it, so we don't know!)
                        save = true;
                        stat = '!';
                    }

                    // this is ok if they aren't hashable.
                    // This will always be true
                    
                    stat = '!';
                    save = false;
                }
                else
                {
                    // doesn't exist on disk yet
                    stat = '*';
                    save = true;
                }
            }

		    print_global_output("        %?  %?\n", stat, data.first);

            if(save)
                to_save[cd.first].push_back(data.first);
        }
    }

    return to_save;
}


void Checkpoint::save_local_cache(const ModuleManager & mm)
{
    using namespace pulsar::util; // to/from_byte_array

    if(!backend_local_ || !backend_global_)
        throw GeneralException("Nullptr for backend in Checkpoint");

    // we have to lock the module manager for a while
    std::lock_guard<std::mutex> l_mm(mm.mutex_);

    // print out some info and get what we should be checkpointing
    auto to_save = form_local_cache_save_list_(mm, *backend_local_);

    // Actually save the data
    for(const auto & mod : to_save)
    {
        std::string modkey = mod.first;

        const auto & modcache = mm.cachemap_.at(modkey);

        for(const auto & cachekey : mod.second)
        {
            const auto & cdat = modcache.cmap_.at(cachekey);
            const auto & gb = *(cdat.value);
            unsigned int policy = cdat.policy;
            const std::string fullkey = make_cache_key_(modkey, cachekey);
            

            // serialize the data
            ByteArray ba_data = gb.to_byte_array();

            bphash::HashValue h;
            if(gb.is_hashable())
                h = gb.my_hash();

            // construct the metadata
            CacheEntryMetadata meta{modkey, cachekey, h, ba_data.size(), policy};
            ByteArray ba_meta = util::to_byte_array(meta);

            print_global_output("Saving: (%10? bytes)   %-30?  %?\n", ba_data.size(), modkey, cachekey);

            // write to the backend
            backend_local_->write(fullkey, ba_meta, ba_data); 
        }
    }
}


} // close namespace modulemanager
} // close namespace pulsar

