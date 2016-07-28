/*! \file
 *
 * \brief Checkpointing of a calcualtion (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_MODULEMANAGER__CHECKPOINT_HPP_
#define PULSAR_GUARD_MODULEMANAGER__CHECKPOINT_HPP_

#include <string>
#include <fstream>
#include <map>

#include <bphash/Hash.hpp>

namespace pulsar {

namespace datastore {
class CacheData;
}

namespace modulemanager {


class ModuleManager;


/*! \brief Handles checkpointing of a module manager
 */
class Checkpoint
{
    public:
        Checkpoint(const std::string & path);

        ~Checkpoint() = default;


        // no copy construction or assignment
        Checkpoint(const Checkpoint & rhs)             = delete;
        Checkpoint(Checkpoint && rhs)                  = default;
        Checkpoint & operator=(const Checkpoint & rhs) = delete;
        Checkpoint & operator=(Checkpoint && rhs)      = default;


        void save(const ModuleManager & mm);
        void load(ModuleManager & mm);


    private:

        ///@{ \name Table of contents

        /*! \brief Data object stored in the checkpoint file TOC */
        struct TOCEntry
        {
            unsigned long modid;
            std::string cachekey;
            bphash::HashValue hash;
            std::string type;
            size_t pos;
            size_t size;
            unsigned int policy;

            template<typename Archive>
            void serialize(Archive & ar)
            {
                ar(modid, cachekey, hash, type, pos, size, policy);
            }
        };

        //! The table of contents
        std::vector<TOCEntry> toc_;

        bool toc_has_hash(const bphash::HashValue & h) const;

        bool toc_has_entry(unsigned long modid, const std::string & cachekey,
                           const bphash::HashValue & h) const;

        const TOCEntry & get_toc_entry(const bphash::HashValue & h) const;

        ///@}

        std::string path_;

        /*! \brief Maps a module's key in the cache map to a unique id */
        unsigned long cur_modid_;
        std::map<std::string, unsigned long> modid_map_;

        void save_module_cache_(const datastore::CacheData & cd, unsigned long modid, std::ofstream & of);
};

} // close namespace modulemanager
} // close namespace pulsar


#endif
