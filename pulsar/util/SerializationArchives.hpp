/*! \file
 *
 * \brief Storage of serialized data
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_UTIL__SERIALIZATIONARCHIVES_HPP_
#define PULSAR_GUARD_UTIL__SERIALIZATIONARCHIVES_HPP_

#include <sstream>
#include <fstream>
#include <cereal/cereal.hpp>
#include <cereal/access.hpp>
#include <cereal/archives/binary.hpp>

#include "pulsar/exception/Exceptions.hpp"


namespace pulsar{
namespace util {

namespace detail {


/*! \brief Base class for archives in Pulsar
 *
 * This provides the common functionality for data storage in Pulsar.
 * Data can be serialized to/unserialized from objects derived from this
 */
template<typename STREAM>
class StdStreamArchive
{
    public:
        /*! \brief Start serialization into storage
         * 
         * \throw pulsar::exception::SerializationException if the object is
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

            oarchive_ = std::unique_ptr<cereal::BinaryOutputArchive>(new cereal::BinaryOutputArchive(stream_));
        }



        /*! \brief Add data to the archive
         * 
         * The objects must be serializable
         * 
         * \throw pulsar::exception::SerializationException if we are not serializing or we
         *        are unserializing.
         */
        template<typename... Targs>
        void Serialize(const Targs &... args)
        {
            if(!serializing_)
                throw exception::SerializationException("Serialize called when we aren't serializing");
            if(unserializing_)
                throw std::logic_error("Serializing and unserializing at the same time");
            if(!oarchive_)
                throw std::logic_error("OArchive is not valid, but we are serializing");

            Serialize_(args...);
        }


        /*! \brief Stop serialization
         * 
         * \return The hash of all the serialized data
         * 
         * \throw pulsar::exception::SerializationException if the object is
         *        unserializing or if it is not serializing to begin with.
         */
        void EndSerialization(void)
        {
            if(serializing_ == false)
                throw exception::SerializationException("Can't stop serializing - Not serializing");
            if(unserializing_ == true)
                throw exception::SerializationException("Can't stop serializing - Currently unserializing");
            if(!oarchive_)
                throw std::logic_error("OArchive is not valid, but we are ending serialization");

            serializing_ = false;
            oarchive_.reset();
            Reset_();
        }



        /*! \brief Start unserialization of stored data
         * 
         * \throw pulsar::exception::SerializationException if the object is
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
         * \throw pulsar::exception::SerializationException if we are serializing or we
         *        are not unserializing.
         */
        template<typename... Targs>
        void Unserialize(Targs &... args)
        {
            if(!unserializing_)
                throw exception::SerializationException("Unserialize called when we aren't unserializing");
            if(serializing_)
                throw std::logic_error("Serializing and unserializing at the same time");
            if(!iarchive_)
                throw std::logic_error("IArchive is not valid, but we are unserializing");

            Unserialize_(args...);
        }



        /*! \brief Extract data from the archive
         * 
         * Returns a single object. Useful for types without a public default constructor
         * 
         * \throw pulsar::exception::SerializationException if we are serializing or we
         *        are not unserializing.
         */
        template<typename T>
        T UnserializeSingle(void)
        {
            T ret;
            Unserialize(ret);
            return ret;
        }



        /*! \brief Stop unserialization
         * 
         * \throw pulsar::exception::SerializationException if the object is
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


        /*! \brief Get the contents of this archive as a byte array */
        std::vector<char> ToByteArray(void)
        {
            BeginUnserialization(); // perform checks and reset positions
            std::vector<char> ret;
            size_t size = Size();
            ret.resize(size);
            stream_.read(ret.data(), size);
            EndUnserialization();
            return ret;
        }

        /*! \brief Create this archive from a byte array */
        void FromByteArray(const std::vector<char> & arr)
        {
            BeginSerialization(); // perform checks and reset positions
            stream_.write(arr.data(), arr.size());
            EndSerialization();
        }


    protected:
        StdStreamArchive() = default;

        STREAM stream_;
        bool serializing_ = false;
        bool unserializing_ = false;


    private:
        std::unique_ptr<cereal::BinaryInputArchive> iarchive_;
        std::unique_ptr<cereal::BinaryOutputArchive> oarchive_;

        void Reset_(void)
        {
            stream_.seekg(0, stream_.beg);
            stream_.seekp(0, stream_.beg);
        }

        void Serialize_(void) { }

        template<typename T>
        void Serialize_(const T & obj)
        {
            (*oarchive_)(obj);
        }


        template<typename T, typename... Targs>
        void Serialize_(T & obj, Targs &... args)
        {
            Serialize_(obj);
            Serialize_(args...);
        }


        void Unserialize_(void) { }

        template<typename T>
        void Unserialize_(T & obj)
        {
            (*iarchive_)(obj);
        }


        template<typename T, typename... Targs>
        void Unserialize_(T & obj, Targs &... args)
        {
            Unserialize_(obj);
            Unserialize_(args...);
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
} // close namespace pulsar



#endif

