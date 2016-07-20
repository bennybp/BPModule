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


namespace pulsar {
namespace modulemanager {


Checkpoint::Checkpoint(const std::string & path)
    : path_(path)
{ }


void Checkpoint::save(const ModuleManager & mm)
{
    print_global_output("Starting checkpoint at %?\n", util::timestamp_string());

    std::lock_guard<std::mutex> l_mm(mm.mutex_);

    print_global_output("Modules in use:\n");
    for(const auto & it : mm.modules_inuse_)
        print_global_output("    %?\n", it);

    sleep(5);

    print_global_output("Checkpoint finished at %?\n", util::timestamp_string());
}


} // close namespace modulemanager
} // close namespace pulsar

