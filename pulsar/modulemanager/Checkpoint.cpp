/*! \file
 *
 * \brief  checkpointing of a calculation (source)
 */

#include "pulsar/modulemanager/Checkpoint.hpp"
#include "pulsar/modulemanager/CheckpointIO.hpp"
#include "pulsar/modulemanager/ModuleManager.hpp"
#include "pulsar/output/GlobalOutput.hpp"
#include "pulsar/util/Serialization.hpp"
#include "pulsar/parallel/Parallel.hpp"

using namespace pulsar;



namespace {

struct CacheEntryMetadata
{
    std::string cachekey;
    std::string type;
    bphash::HashValue hash;  //!< Hash of original data, NOT serialized data
    size_t size;             //!< Size of serialized data
    unsigned int policy;     //!< Storage policy

    template<typename Archive>
    void serialize(Archive & ar)
    {
        ar(cachekey, type, hash, size, policy);
    }
};

bool is_meta_key(const std::string & s)
{
    if(s.size() <= 6)
        return false; // needs at least one char + "##META"

    std::string last5 = s.substr(s.size()-6, 6);

    if(last5 == "##META")
        return true;
    else
        return false;
}

bool is_cache_key(const std::string & s)
{
    if(s.size() <= 13)
        return false; // Needs at least "CHKPT_CACHE__" + one char

    std::string first13 = s.substr(0, 13);

    if(first13 != "CHKPT_CACHE__")
        return false;
    else
        return true;
}


std::string make_cache_key(const std::string & datakey)
{
    return std::string("CHKPT_CACHE__") + datakey;
}

std::string
split_cache_key(const std::string & key)
{
    if(!is_cache_key(key))
        throw pulsar::PulsarException("Unknown key format: not a cache key",
                               "key", key);

    return key.substr(13); // remove "CHKPT_CACHE__"
}


} // close anonymous namespace



namespace pulsar {


Checkpoint::Checkpoint(const std::shared_ptr<CheckpointIO> & backend_local,
                       const std::shared_ptr<CheckpointIO> & backend_global)
    : backend_local_(backend_local),
      backend_global_(backend_global)
{
    if(backend_local == backend_global)
        throw PulsarException("Local and Global backends are the same! This will lead to problems");
}



        
std::vector<std::string>
Checkpoint::form_cache_save_list_(const ModuleManager & mm,
                                  CheckpointIO & backend,
                                  std::function<bool(unsigned int)> policy_check)
{
    using pulsar::detail::GenericHolder;
    using pulsar::detail::SerializedGenericData;

    // modulemanager should be locked already!
    // print out some info and get what we should be checkpointing
    print_global_output("Cache entries in the module manager:\n");

    std::vector<std::string> to_save;

    for(const auto & data : mm.cachemap_.cmap_)
    {
        const auto * gb = data.second.value.get();

        bool save = false;
        char stat = ' ';

        // is this actually serialized data?
        const GenericHolder<SerializedGenericData> * sd = dynamic_cast<const GenericHolder<SerializedGenericData> *>(gb);
        bool is_serialized = (sd != nullptr);

        if(policy_check(data.second.policy) && (is_serialized || gb->is_serializable()))
        {
            std::string full_key = make_cache_key(data.first);
            std::string full_metakey = full_key + "##META";

            // does the data already exist in the checkpoint backend?
            if(backend.count(full_key))
            {
                // if it's hashable, read the metadata and compare the hashes
                if(gb->is_hashable())
                {
                    ByteArray ba_meta = backend.read(full_metakey);
                    auto meta = from_byte_array<CacheEntryMetadata>(ba_meta);

                    auto newhash = gb->my_hash();
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
            to_save.push_back(data.first);
    }

    return to_save;
}



void Checkpoint::save_cache_(const ModuleManager & mm,
                             CheckpointIO & backend,
                             std::function<bool(unsigned int)> policy_check)
{
     // to/from_byte_array

    // Actually save the data
    backend.open();

    // we have to lock the module manager for a while
    std::lock_guard<std::mutex> l_mm(mm.mutex_);

    // print out some info and get what we should be checkpointing
    auto to_save = form_cache_save_list_(mm, backend, policy_check);

    try {

        for(const auto & cachekey : to_save)
        {
            const auto & cdat = mm.cachemap_.cmap_.at(cachekey);
            const auto & gb = *(cdat.value);
            unsigned int policy = cdat.policy;
            const std::string full_key = make_cache_key(cachekey);
            const std::string full_metakey = full_key + "##META";
            
            // serialize the data
            ByteArray ba_data = gb.to_byte_array();

            bphash::HashValue h;
            if(gb.is_hashable())
                h = gb.my_hash();

            // construct the metadata
            CacheEntryMetadata meta{cachekey, 
                                    gb.type(),
                                    h,
                                    ba_data.size(),
                                    policy};
            ByteArray ba_meta = to_byte_array(meta);

            print_global_output("Saving: (%10? bytes)   %?\n", ba_data.size(), cachekey);

            // write to the backend
            backend.write(full_key, ba_data); 
            backend.write(full_metakey, ba_meta); 
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
    using namespace pulsar::detail;
     // to/from_byte_array

    // not sure if I really have to lock mm, byt why not
    std::lock_guard<std::mutex> l_mm(mm.mutex_);
    std::lock_guard<std::mutex> c_mm(mm.cachemap_.mutex_);

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
                const std::string metakey = it + "##META";

                std::string cachekey = split_cache_key(it);
                print_global_output("Loading %?\n", cachekey);

                ByteArray data = backend.read(it);
                ByteArray metadata = backend.read(metakey);
                auto cem = from_byte_array<CacheEntryMetadata>(metadata);

                SerializedGenericData scd{std::move(data), cem.type, cem.hash, cem.policy};
                std::unique_ptr<GenericHolder<SerializedGenericData>> sdh(new GenericHolder<SerializedGenericData>(std::move(scd)));
                mm.cachemap_.set_(cachekey, std::move(sdh), cem.policy); // set_ won't lock the mutex
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
    const long my_rank = get_proc_id();
    const long nproc = get_nproc();


    for(long rank = 0; rank < nproc; rank++)
    {
        if(rank == my_rank)
        {
            // perform the action
            print_global_output("On rank %?: ", my_rank);
            print_global_output(description + "\n");
            func(); // call the function
        }

        MPI_Barrier(MPI_COMM_WORLD); //! \todo different comm?
    }
}


void Checkpoint::save_local_cache(const ModuleManager & mm)
{
    if(!backend_local_ || !backend_global_)
        throw PulsarException("Nullptr for backend in Checkpoint");

    print_global_output("Saving local cache\n");
    save_cache_(mm, *backend_local_, 
                    [](unsigned int pol) { return pol & CacheMap::CheckpointLocal; });
}

void Checkpoint::load_local_cache(ModuleManager & mm)
{
    if(!backend_local_ || !backend_global_)
        throw PulsarException("Nullptr for backend in Checkpoint");

    print_global_output("Loading local cache\n");
    load_cache_(mm, *backend_local_);
}



void Checkpoint::save_global_cache(const ModuleManager & mm)
{
    if(!backend_local_ || !backend_global_)
        throw PulsarException("Nullptr for backend in Checkpoint");

    std::function<void(void)> func = std::bind(&Checkpoint::save_cache_, this,
                                               std::ref(mm), std::ref(*backend_global_),
                                               [](unsigned int pol) { return pol & CacheMap::CheckpointGlobal; });

    perform_on_all_ranks_("Saving global cache", func);
}

void Checkpoint::load_global_cache(ModuleManager & mm)
{
    if(!backend_local_ || !backend_global_)
        throw PulsarException("Nullptr for backend in Checkpoint");

    std::function<void(void)> func = std::bind(&Checkpoint::load_cache_, this,
                                               std::ref(mm), std::ref(*backend_global_));

    perform_on_all_ranks_("Loading global cache", func);
}

} // close namespace pulsar

