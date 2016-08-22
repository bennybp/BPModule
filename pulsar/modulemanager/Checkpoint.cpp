/*! \file
 *
 * \brief  checkpointing of a calculation (source)
 */

#include "pulsar/modulemanager/Checkpoint.hpp"
#include "pulsar/modulemanager/CheckpointIO.hpp"
#include "pulsar/modulemanager/ModuleManager.hpp"
#include "pulsar/output/GlobalOutput.hpp"
#include "pulsar/util/Serialization.hpp"
#include "pulsar/parallel/InitFinalize.hpp"

using namespace pulsar::exception;
using namespace pulsar::datastore;
using namespace pulsar::modulemanager;
using namespace pulsar::output;


namespace {

struct CacheEntryMetadata
{
    std::string modkey;
    std::string cachekey;
    std::string type;
    bphash::HashValue hash;  //!< Hash of original data, NOT serialized data
    size_t size;             //!< Size of serialized data
    unsigned int policy;     //!< Storage policy

    template<typename Archive>
    void serialize(Archive & ar)
    {
        ar(modkey, cachekey, type, hash, size, policy);
    }
};

static bool is_meta_key(const std::string & s)
{
    if(s.size() <= 6)
        return false; // needs at least one char + "##META"

    std::string last5 = s.substr(s.size()-6, 6);

    if(last5 == "##META")
        return true;
    else
        return false;
}

static bool is_cache_key(const std::string & s)
{
    if(s.size() <= 13)
        return false; // Needs at least "CHKPT_CACHE__" + one char

    std::string first13 = s.substr(0, 13);

    if(first13 != "CHKPT_CACHE__")
        return false;

    // also needs a "_##_" in the middle
    if(s.find("_##_") != std::string::npos)
        return true;
    else
        return false;
}


static std::string make_cache_key(const std::string & modkey,
                                  const std::string & datakey)
{
    return std::string("CHKPT_CACHE__") + modkey + "_##_" + datakey;
}

std::pair<std::string, std::string>
split_cache_key(const std::string & key)
{
    if(!is_cache_key(key))
        throw GeneralException("Unknown key format: not a cache key",
                               "key", key);

    std::string real_key = key.substr(13); // remove "CHKPT_CACHE__"
    size_t sep_pos = real_key.find("_##_");
    return {real_key.substr(0, sep_pos),
            real_key.substr(sep_pos+4)};
    
}


}



namespace pulsar {
namespace modulemanager {


Checkpoint::Checkpoint(const std::shared_ptr<CheckpointIO> & backend_local,
                       const std::shared_ptr<CheckpointIO> & backend_global)
    : backend_local_(backend_local),
      backend_global_(backend_global)
{
    if(backend_local == backend_global)
        throw GeneralException("Local and Global backends are the same! This will lead to problems");
}



        
std::map<std::string, std::vector<std::string>>
Checkpoint::form_cache_save_list_(const ModuleManager & mm,
                                  CheckpointIO & backend,
                                  std::function<bool(unsigned int)> policy_check)
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

            if(policy_check(data.second.policy) && gb.is_serializable())
            {
                std::string full_key = make_cache_key(cd.first, data.first);
                std::string full_metakey = full_key + "##META";

                // does the data already exist?
                if(backend.count(full_key))
                {
                    // if it's hashable, read the metadata and compare the hashes
                    if(gb.is_hashable())
                    {
                        ByteArray ba_meta = backend.read(full_metakey);
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



void Checkpoint::save_cache_(const ModuleManager & mm,
                             CheckpointIO & backend,
                             std::function<bool(unsigned int)> policy_check)
{
    using namespace pulsar::util; // to/from_byte_array

    // Actually save the data
    backend.open();

    // we have to lock the module manager for a while
    std::lock_guard<std::mutex> l_mm(mm.mutex_);

    // print out some info and get what we should be checkpointing
    auto to_save = form_cache_save_list_(mm, backend, policy_check);

    try {

        for(const auto & mod : to_save)
        {
            std::string modkey = mod.first;

            const auto & modcache = mm.cachemap_.at(modkey);

            for(const auto & cachekey : mod.second)
            {
                const auto & cdat = modcache.cmap_.at(cachekey);
                const auto & gb = *(cdat.value);
                unsigned int policy = cdat.policy;
                const std::string full_key = make_cache_key(modkey, cachekey);
                const std::string full_metakey = full_key + "##META";
                
                // serialize the data
                ByteArray ba_data = gb.to_byte_array();

                bphash::HashValue h;
                if(gb.is_hashable())
                    h = gb.my_hash();

                // construct the metadata
                CacheEntryMetadata meta{modkey, cachekey, 
                                        gb.type(), h,
                                        ba_data.size(), policy};
                ByteArray ba_meta = util::to_byte_array(meta);

                print_global_output("Saving: (%10? bytes)   %-30?  %?\n", ba_data.size(), modkey, cachekey);

                // write to the backend
                backend.write(full_key, ba_data); 
                backend.write(full_metakey, ba_meta); 
            }
        }
    }
    catch(...)
    {
        backend.close();
        throw;
    }

    backend.close();
}


void Checkpoint::load_cache_(ModuleManager & mm, CheckpointIO & backend)
{
    using namespace pulsar::datastore::detail;
    using namespace pulsar::util; // to/from_byte_array

    std::lock_guard<std::mutex> l_mm(mm.mutex_);

    backend.open();

    try {
        auto all_keys = backend.all_keys();

        print_global_output("Checkpoint keys:\n");
        for(const auto & key : all_keys)
            print_global_output("    %?\n", key);

        // load the data
        for(const auto & it : all_keys)
        {
            if(is_cache_key(it) && !is_meta_key(it))
            {
                auto key_components = split_cache_key(it);
                const std::string modkey = key_components.first;
                const std::string cachekey = key_components.second;
                print_global_output("Loading %?   %?\n", modkey, cachekey);

                const std::string metakey = it + "##META";

                ByteArray data = backend.read(it);
                ByteArray metadata = backend.read(metakey);
                auto cem = from_byte_array<CacheEntryMetadata>(metadata);

                SerializedCacheData scd{std::move(data), cem.type, cem.hash};
                auto pscd = std::make_shared<SerializedCacheData>(std::move(scd));
                std::unique_ptr<SerializedDataHolder> sdh(new SerializedDataHolder(pscd));
                mm.cachemap_[modkey].set_(cachekey, std::move(sdh), cem.policy);
            }
        }
    }
    catch(...)
    {
        backend.close();
        throw;
    }

    backend.close();
}



void Checkpoint::perform_on_all_ranks_(const std::string & description, std::function<void(void)> func)
{
    const long my_rank = parallel::get_proc_id();
    const long nproc = parallel::get_nproc();

    if(my_rank == 0)
    {
        print_global_output("On master: ");
        print_global_output(description + "\n");

        func(); // call the function

        if(nproc > 1)
        {
            long dest = 1;

            // send a note to rank 1 to let it know
            // it can safely use the global file
            MPI_Send(&dest, 1, MPI_LONG_INT, dest, 283, MPI_COMM_WORLD);
        }
    }
    else
    {
        // wait for the note from the previous rank
        long r;
        long src = my_rank - 1;

        MPI_Status status;

        MPI_Recv(&r, 1, MPI_LONG_INT, src, 283, MPI_COMM_WORLD, &status);

        //! \todo if r != my_rank, then what?
        
        print_global_output("On rank %?: ", my_rank);
        print_global_output(description + "\n");
        func(); // call the function


        long dest = my_rank + 1;
        if(dest < nproc)
        {
            // send a note to the next node to let it know
            // it can safely use the global file
            MPI_Send(&dest, 1, MPI_LONG_INT, dest, 283, MPI_COMM_WORLD);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD); //! \todo is this really necessary
}


void Checkpoint::save_local_cache(const ModuleManager & mm)
{
    if(!backend_local_ || !backend_global_)
        throw GeneralException("Nullptr for backend in Checkpoint");

    print_global_output("Saving local cache\n");
    save_cache_(mm, *backend_local_, 
                    [](unsigned int pol) { return pol & CacheData::CheckpointLocal; });
}

void Checkpoint::load_local_cache(ModuleManager & mm)
{
    if(!backend_local_ || !backend_global_)
        throw GeneralException("Nullptr for backend in Checkpoint");

    print_global_output("Loading local cache\n");
    load_cache_(mm, *backend_local_);
}



void Checkpoint::save_global_cache(const ModuleManager & mm)
{
    if(!backend_local_ || !backend_global_)
        throw GeneralException("Nullptr for backend in Checkpoint");

    std::function<void(void)> func = std::bind(&Checkpoint::save_cache_, this,
                                               std::ref(mm), std::ref(*backend_global_),
                                               [](unsigned int pol) { return pol & CacheData::CheckpointGlobal; });

    perform_on_all_ranks_("Saving global cache", func);
}

void Checkpoint::load_global_cache(ModuleManager & mm)
{
    if(!backend_local_ || !backend_global_)
        throw GeneralException("Nullptr for backend in Checkpoint");

    std::function<void(void)> func = std::bind(&Checkpoint::load_cache_, this,
                                               std::ref(mm), std::ref(*backend_global_));

    perform_on_all_ranks_("Loading global cache", func);
}


} // close namespace modulemanager
} // close namespace pulsar

