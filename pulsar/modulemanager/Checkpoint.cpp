/*! \file
 *
 * \brief Checkpointing of a calcualtion (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "pulsar/modulemanager/Checkpoint.hpp"

namespace pulsar{
namespace modulemanager {


Checkpoint::Checkpoint(const std::string & path)
    : path_(path)
{ }


} // close namespace modulemanager
} // close namespace pulsar

