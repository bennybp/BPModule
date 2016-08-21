/*! \file
 *
 * \brief File checkpointing backend using Berkeley DB
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#pragma once

#include "pulsar/modulemanager/CheckpointIO.hpp"
#include <db.h> // BerkeleyDB C API

namespace pulsar {
namespace modulemanager {


/*! \brief Interface to an IO backend for checkpointing
 *
 * \note This uses the C api. I know BerkeleyDB has a C++ API, but
 *       it doesn't add much, and tends to throw exceptions when
 *       I don't want it to.
 */
class BDBCheckpointIO : public CheckpointIO
{
    public:
        BDBCheckpointIO(const std::string & path, bool truncate);
        ~BDBCheckpointIO();


        // no copy construction or assignment
        BDBCheckpointIO() = delete;
        BDBCheckpointIO(const BDBCheckpointIO & rhs)             = delete;
        BDBCheckpointIO(BDBCheckpointIO && rhs)                  = default;
        BDBCheckpointIO & operator=(const BDBCheckpointIO & rhs) = delete;
        BDBCheckpointIO & operator=(BDBCheckpointIO && rhs)      = default;

        virtual size_t count(const std::string & key) const;

        virtual std::set<std::string> all_keys(void) const;

        virtual void write(const std::string & key, const ByteArray & data);

        virtual ByteArray read(const std::string & key) const;

        virtual void erase(const std::string & key);

        virtual void clear(void);

    private:
        const std::string path_;
        std::set<std::string> stored_keys_;
        DB * db_;

        void read_keys_(void);

        void write_(const std::string & key, const ByteArray & data,
                    bool add_key);
};

} // close namespace modulemanager
} // close namespace pulsar

