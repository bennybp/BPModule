/*! \file
 *
 * \brief Checkpointing of a calcualtion (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_MODULEMANAGER__CHECKPOINT_HPP_
#define PULSAR_GUARD_MODULEMANAGER__CHECKPOINT_HPP_

#include <string>

namespace pulsar{
namespace modulemanager {


class ModuleManager;


/*! \brief Handles checkpointing of a module manager
 */
class Checkpoint
{
    public:
        Checkpoint(const std::string & path);

        ~Checkpoint();


        // no copy construction or assignment
        Checkpoint(const Checkpoint & rhs)             = delete;
        Checkpoint(Checkpoint && rhs)                  = default;
        Checkpoint & operator=(const Checkpoint & rhs) = delete;
        Checkpoint & operator=(Checkpoint && rhs)      = default;


    private:
        std::string path_;
};



} // close namespace modulemanager
} // close namespace pulsar


#endif
