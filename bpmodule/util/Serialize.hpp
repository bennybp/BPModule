/*! \file
 *
 * \brief Serialization helpers
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef BPMODULE_GUARD_UTIL__SERIALIZE_HPP_
#define BPMODULE_GUARD_UTIL__SERIALIZE_HPP_

#include <sstream>
#include <fstream>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>

namespace bpmodule {
namespace util {

namespace detail {

template<typename STREAM>
class StdStreamArchive
{
    public:
        StdStreamArchive() = default;
        StdStreamArchive(const StdStreamArchive &) = delete;
        StdStreamArchive & operator=(const StdStreamArchive &) = delete;
        StdStreamArchive(StdStreamArchive &&) = default;
        StdStreamArchive & operator=(StdStreamArchive &&) = default;

        template<typename T>
        void Serialize(const T & obj)
        {
            cereal::BinaryOutputArchive oarchive(stream_);
            oarchive(obj);
        }

        template<typename T>
        void Unserialize(T & obj)
        {
            cereal::BinaryInputArchive iarchive(stream_);
            iarchive(obj);
        }

        void Reset(void)
        {
            stream_.seekg(0, stream_.end);
            stream_.seekp(0, stream_.beg);
        }

        size_t Size(void)
        {
            // save the original output position
            std::streampos orig = stream_.tellp();
            stream_.seekp(0, stream_.end);
            size_t size = stream_.tellp();

            // set the output position back to where it was
            stream_.seekp(orig);
            return size;
        }
 

    protected:
        STREAM stream_;
};

} // close namespace detail


/// Serialization of data to/from memory
typedef detail::StdStreamArchive<std::stringstream> MemoryArchive;


/// Serialization of data to/from a file
class FileArchive : public detail::StdStreamArchive<std::fstream>
{
    public:
        FileArchive(const std::string & path,
                    std::ios_base::openmode mode = std::fstream::out |
                                                   std::fstream::in |
                                                   std::fstream::trunc)
        {
            stream_.open(path.c_str(), mode);
            if(!stream_.good())
                throw std::runtime_error("HERE");
            Reset();
        }
};



} // close namespace util
} // close namespace bpmodule


#endif

