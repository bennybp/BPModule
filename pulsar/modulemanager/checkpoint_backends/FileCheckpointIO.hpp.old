/*! \file
 *
 * \brief File checkpointing backend
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#pragma once

#include "pulsar/modulemanager/CheckpointIO.hpp"

#include <map>
#include <fstream>

namespace pulsar {
namespace modulemanager {


/*! \brief Interface to an IO backend for checkpointing
 */
class FileCheckpointIO : public CheckpointIO
{
    public:
        FileCheckpointIO(const std::string & path, bool truncate);
        ~FileCheckpointIO() = default;


        // no copy construction or assignment
        FileCheckpointIO() = delete;
        FileCheckpointIO(const FileCheckpointIO & rhs)             = delete;
        FileCheckpointIO(FileCheckpointIO && rhs)                  = default;
        FileCheckpointIO & operator=(const FileCheckpointIO & rhs) = delete;
        FileCheckpointIO & operator=(FileCheckpointIO && rhs)      = default;

        virtual size_t size(void) const;

        virtual size_t count(const std::string & key) const;

        virtual std::set<std::string> all_keys(void) const;

        virtual void write(const std::string & key,
                           const ByteArray & metadata,
                           const ByteArray & data);

        virtual std::pair<ByteArray, ByteArray> read(const std::string & key) const;

        virtual ByteArray read_metadata(const std::string & key) const;

        virtual void erase(const std::string & key);

        virtual void clear(void);

    private:
        struct FileTOCEntry
        {
            std::string key;
            size_t metadata_size;
            size_t data_size;
            std::streampos pos;

            template<typename Archive>
            void serialize(Archive & ar)
            {
                // the pos member is not serialized - that is handled
                // on reading/writing
                ar(key, metadata_size, data_size);
            }
        };


        const std::string path_;
        mutable std::fstream file_;

        std::map<std::string, FileTOCEntry> toc_;
};

} // close namespace modulemanager
} // close namespace pulsar

