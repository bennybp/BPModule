/*! \file
 *
 * \brief Interface to an IO backend for checkpointing
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#pragma once

#include "pulsar/util/Serialization_fwd.hpp"

#include <string>
#include <set>

namespace pulsar {
namespace modulemanager {

/*! \brief Interface to an IO backend for checkpointing
 */
class CheckpointIO
{
    public:
        CheckpointIO() = default;
        ~CheckpointIO() = default;


        // no copy construction or assignment
        CheckpointIO(const CheckpointIO & rhs)             = delete;
        CheckpointIO(CheckpointIO && rhs)                  = default;
        CheckpointIO & operator=(const CheckpointIO & rhs) = delete;
        CheckpointIO & operator=(CheckpointIO && rhs)      = default;

        virtual size_t count(const std::string & key) const = 0;

        virtual std::set<std::string> all_keys(void) const = 0;

        virtual void write(const std::string & key, const ByteArray & data) = 0;

        virtual ByteArray read(const std::string & key) const = 0;

        virtual void erase(const std::string & key) = 0;

        virtual void clear(void) = 0; 
};

} // close namespace modulemanager
} // close namespace pulsar

