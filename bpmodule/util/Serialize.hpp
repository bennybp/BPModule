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

#include "bpmodule/util/HashingArchive.hpp"
#include "bpmodule/exception/Exceptions.hpp"


namespace bpmodule {
namespace util {

namespace detail {


/*! \brief Base class for archives in BPModule
 *
 * This provides the common functionality for data storage in BPModule.
 * Data can be serialized to/unserialized from objects derived from this
 */
template<typename STREAM>
class StdStreamArchive
{
    public:
        /*! \brief Start serialization into storage
         * 
         * \throw bpmodule::exception::SerializationException if the object is
         *        already serializing or unserializing
         */
        void BeginSerialization(void)
        {
            if(serializing_ == true)
                throw exception::SerializationException("Can't start serializing - Already serializing");
            if(unserializing_ == true)
                throw exception::SerializationException("Can't start serializing - Currently unserializing");
            if(oarchive_)
                throw std::logic_error("OArchive is valid, but we aren't doing anything");

            serializing_ = true;
            Reset_();

            oarchive_ = std::unique_ptr<detail::HashingOutputArchive>(new detail::HashingOutputArchive(stream_));
        }


        /*! \brief Add data to the archive
         * 
         * The object must be serializable
         * 
         * \throw bpmodule::exception::SerializationException if we are not serializing or we
         *        are unserializing.
         */
        template<typename T>
        void Serialize(const T & obj)
        {
            if(!serializing_)
                throw exception::SerializationException("Serialize called when we aren't serializing");
            if(unserializing_)
                throw std::logic_error("Serializing and unserializing at the same time");
            if(!oarchive_)
                throw std::logic_error("OArchive is not valid, but we are serializing");

            (*oarchive_)(obj);
        }


        /*! \brief Stop serialization
         * 
         * \return The hash of all the serialized data
         * 
         * \throw bpmodule::exception::SerializationException if the object is
         *        unserializing or if it is not serializing to begin with.
         */
        Hash EndSerialization(void)
        {
            if(serializing_ == false)
                throw exception::SerializationException("Can't stop serializing - Not serializing");
            if(unserializing_ == true)
                throw exception::SerializationException("Can't stop serializing - Currently unserializing");
            if(!oarchive_)
                throw std::logic_error("OArchive is not valid, but we are ending serialization");

            serializing_ = false;
            Hash hash = oarchive_->Finalize();
            oarchive_.reset();
            Reset_();
            return hash; 
        }



        /*! \brief Start unserialization of stored data
         * 
         * \throw bpmodule::exception::SerializationException if the object is
         *        already serializing or unserializing
         */
        void BeginUnserialization(void)
        {
            if(serializing_ == true)
                throw exception::SerializationException("Can't start unserializing - Currently serializing");
            if(unserializing_ == true)
                throw exception::SerializationException("Can't start unserializing - Already unserializing");
            if(iarchive_)
                throw std::logic_error("IArchive is valid, but we aren't doing anything");

            unserializing_ = true;
            Reset_();

            iarchive_ = std::unique_ptr<cereal::BinaryInputArchive>(new cereal::BinaryInputArchive(stream_));
        }


        /*! \brief Extract data from the archive
         * 
         * \throw bpmodule::exception::SerializationException if we are serializing or we
         *        are not unserializing.
         */
        template<typename T>
        void Unserialize(T & obj)
        {
            if(!unserializing_)
                throw exception::SerializationException("Unserialize called when we aren't unserializing");
            if(serializing_)
                throw std::logic_error("Serializing and unserializing at the same time");
            if(!iarchive_)
                throw std::logic_error("IArchive is not valid, but we are unserializing");

            (*iarchive_)(obj);
        }

        /*! \brief Stop unserialization
         * 
         * \throw bpmodule::exception::SerializationException if the object is
         *        serializing or if it is not unserializing to begin with.
         */
        void EndUnserialization(void)
        {
            if(serializing_ == true)
                throw exception::SerializationException("Can't stop unserializing - Currently serializing");
            if(unserializing_ == false)
                throw exception::SerializationException("Can't stop unserializing - Not unserializing");
            if(!iarchive_)
                throw std::logic_error("IArchive is not valid, but we are ending unserialization");

            unserializing_ = false;;
            iarchive_.reset();
            Reset_();
        }


        /*! \brief Obtain the current size of stored data (in bytes)
         */ 
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
        StdStreamArchive() = default;

        STREAM stream_;
        bool serializing_ = false;
        bool unserializing_ = false;


    private:
        std::unique_ptr<cereal::BinaryInputArchive> iarchive_;
        std::unique_ptr<detail::HashingOutputArchive> oarchive_;

        void Reset_(void)
        {
            stream_.seekg(0, stream_.beg);
            stream_.seekp(0, stream_.beg);
        }

};

} // close namespace detail


/// Serialization of data to/from memory
class MemoryArchive : public detail::StdStreamArchive<std::stringstream>
{
    private:
        typedef detail::StdStreamArchive<std::stringstream> Base;
    public:
        // forward the protected constructor
        using Base::Base;
}; 



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
                throw exception::SerializationException("Error opening file");
        }
};



} // close namespace util
} // close namespace bpmodule


#endif

