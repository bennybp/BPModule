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


namespace pulsar {
namespace modulemanager {



Checkpoint::Checkpoint(const std::string & path)
    : path_(path), cur_modid_(0)
{ }


bool Checkpoint::toc_has_hash(const bphash::HashValue & h) const
{
    auto it = std::find_if(toc_.begin(), toc_.end(),
                           [ & h ] (const TOCEntry & e)
                           { return e.hash == h; });

    return it != toc_.end();
}

bool Checkpoint::toc_has_entry(unsigned long modid, const std::string & cachekey,
                               const bphash::HashValue & h) const
{
    auto it = std::find_if(toc_.begin(), toc_.end(),
                           [ modid, & cachekey, & h ] (const TOCEntry & e)
                           { return e.modid == modid &&
                                    e.cachekey == cachekey &&
                                    e.hash == h; });

    return it != toc_.end();
}

const Checkpoint::TOCEntry & Checkpoint::get_toc_entry(const bphash::HashValue & h) const
{
    auto it = std::find_if(toc_.begin(), toc_.end(),
                           [ & h ] (const TOCEntry & e)
                           { return e.hash == h; });

    if(it == toc_.end())
        throw exception::GeneralException("TOC entry not available");
    else
        return *it;
}




void Checkpoint::save(const ModuleManager & mm)
{
    std::lock_guard<std::mutex> l_mm(mm.mutex_);
    std::string metafile = path_ + "/psrtest/chkpt.meta";
    std::string datafile = path_ + "/psrtest/chkpt.dat";

    print_global_output("Starting checkpoint at %?\n", util::timestamp_string());
    print_global_output("   Meta file: %?\n", metafile);
    print_global_output("   Data file: %?\n", datafile);

    //! \todo enable exceptions on the streams
    std::ofstream meta_of(metafile.c_str(), std::fstream::trunc | std::fstream::binary);
    std::ofstream data_of(datafile.c_str(), std::fstream::trunc | std::fstream::binary);

    if(!meta_of.is_open())
        throw exception::GeneralException("Unable to open metatdata file for writing", "file", metafile);
    if(!data_of.is_open())
        throw exception::GeneralException("Unable to open data file for writing", "file", datafile);

    print_global_output("Cache entries in the module manager:\n");
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


        print_global_output("    %4?  %?\n", modid, it.first);
    }

    for(const auto & it : mm.cachemap_)
    {
        print_global_output("++ Checkpointing [%?] %?\n", modid_map_.at(it.first), it.first);
        save_module_cache_(it.second, modid_map_.at(it.first), data_of);
    }


    print_global_output("Writing metadata\n");
    
    {
        cereal::BinaryOutputArchive oar(meta_of);
        oar(toc_, modid_map_);
    }

    print_global_output("Checkpoint finished at %?\n", util::timestamp_string());
}


void Checkpoint::save_module_cache_(const CacheData & cd, unsigned long modid,
                                    std::ofstream & of)
{
    unsigned int nserializable = 0;
    unsigned int nlocal = 0;
    unsigned int nglobal = 0;
    for(const auto & it : cd.cmap_)
    {
        if(it.second.value->is_serializable())
        {
            nserializable++;
            if(it.second.policy & CacheData::CheckpointGlobal)
                nglobal++;
            else if(it.second.policy & CacheData::CheckpointLocal)
                nlocal++;
        }
    }

    print_global_output("     > %? entries, %? nserializable, %? local, %? global\n",
                        cd.size(), nserializable, nlocal, nglobal);

    if(nlocal == 0 && nglobal == 0)
        print_global_output("      skipping...\n");

    for(const auto & it : cd.cmap_)
    {
        if( it.second.value->is_serializable() &&
           (it.second.policy & CacheData::CheckpointGlobal ||
            it.second.policy & CacheData::CheckpointLocal) )
        {
            bool ishashable = it.second.value->is_hashable();

            bphash::HashValue h;
            std::string hashstr = "(not hashable)";
            if(ishashable)
            {
                h = it.second.value->my_hash();
                hashstr = bphash::hash_to_string(h);
            }

            print_global_output("       + saving: %?  hash: %?\n", it.first, hashstr);


            // has this already been written?
            if(toc_has_entry(modid, it.first, h))
            {
                //! \todo overwrite? delete old?
                //        probably not until we have a proper DB backend
                print_global_output("           * existing entry found in TOC. skipping");
                continue;
            }


            // build the toc entry
            TOCEntry te{modid, it.first, h, 0};

            // see if this has already been stored
            bool existing = false;
            if(ishashable)
            {
                if(toc_has_hash(h))
                {
                    // already been stored somewhere
                    // get that position
                    te.pos = static_cast<size_t>(get_toc_entry(h).pos);
                    existing = true;
                }
            }

            if(!existing)
            {
                te.pos = of.tellp();

                // serialize and write
                ByteArray bar = it.second.value->to_byte_array();

                size_t nbytes = bar.size() * sizeof(ByteArray::value_type);
                of.write(bar.data(), nbytes);
                print_global_output("           * wrote %? bytes to position %?\n", nbytes, te.pos);
            }
            else
                print_global_output("           * existing data found. position = %?\n", te.pos);


            // add the toc entry
            toc_.push_back(std::move(te));
        }
    }

    print_global_output("\n");
}



} // close namespace modulemanager
} // close namespace pulsar

