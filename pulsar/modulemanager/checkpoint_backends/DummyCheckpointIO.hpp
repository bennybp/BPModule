/*! \file
 *
 * \brief Dummy checkpointing backend
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#pragma once

#include "pulsar/modulemanager/CheckpointIO.hpp"
#include "pulsar/exception/Exceptions.hpp"

namespace pulsar {
namespace modulemanager {


/*! \brief Interface to an IO backend for checkpointing
 */
class DummyCheckpointIO : public CheckpointIO
{
    public:
        DummyCheckpointIO() = default;
        ~DummyCheckpointIO() = default;


        // no copy construction or assignment
        DummyCheckpointIO(const DummyCheckpointIO & rhs)             = delete;
        DummyCheckpointIO(DummyCheckpointIO && rhs)                  = default;
        DummyCheckpointIO & operator=(const DummyCheckpointIO & rhs) = delete;
        DummyCheckpointIO & operator=(DummyCheckpointIO && rhs)      = default;

        virtual void open(void) { }

        virtual void close(void) { }

        virtual size_t count(const std::string &) const { return 0; }

        virtual std::set<std::string> all_keys(void) const { return {}; }

        virtual void write(const std::string &, const ByteArray &) { }

        virtual ByteArray read(const std::string &) const
        {
            throw GeneralException("Cannot read from dummy IO");
        }

        virtual void erase(const std::string &)
        { }

        virtual void clear(void) { }
};

} // close namespace modulemanager
} // close namespace pulsar

