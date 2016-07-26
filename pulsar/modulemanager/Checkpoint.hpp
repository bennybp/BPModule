/*! \file
 *
 * \brief Checkpointing of a calcualtion (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_MODULEMANAGER__CHECKPOINT_HPP_
#define PULSAR_GUARD_MODULEMANAGER__CHECKPOINT_HPP_

#include <string>

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


    private:
        std::string path_;

        void save_module_cache_(const datastore::CacheData & cd);
};

} // close namespace modulemanager
} // close namespace pulsar


#endif
