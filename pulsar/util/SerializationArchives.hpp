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

#include "pulsar/exception/PulsarException.hpp"
#include "pulsar/util/Serialization_fwd.hpp"


namespace pulsar{
namespace detail {


/*! \brief Base class for archives in Pulsar
 *
 * This provides the common functionality for data storage in Pulsar.
 * Data can be serialized to/unserialized from objects derived from this
 *
 * The class is templated on the archive type in case we want to ever use some
 * of Cereal's other archives (such as JSON or XML).
 *
 * \param[in] STREAM the type of std stream to use as the buffer
 * \param[in] InArchive the cereal input archive type (default: BinaryInputArchive)
 * \param[in] OutArchive the cereal output archive type (default: BinaryOutputArchive)
 */
template<typename STREAM,
         typename InArchive=cereal::BinaryInputArchive,
         typename OutArchive=cereal::BinaryOutputArchive>
class StdStreamArchive
{
    public:
        /*! \brief Start serialization into storage
         * 
         * \throw pulsar::PulsarException if the object is
         *        already serializing or unserializing
         */
        void begin_serialization(void)
        {
            if(serializing_ == true)
                throw PulsarException("Can't start serializing - Already serializing");
            if(unserializing_ == true)
                throw PulsarException("Can't start serializing - Currently unserializing");
            if(oarchive_)
                throw std::logic_error("OArchive is valid, but we aren't doing anything");

            serializing_ = true;
            Reset_();

            oarchive_ = std::unique_ptr<OutArchive>(new OutArchive(stream_));
        }



        /*! \brief Add data to the archive
         * 
         * The objects must be serializable
         * 
         * \throw pulsar::PulsarException if we are not serializing or we
         *        are unserializing.
         */
        template<typename... Targs>
        void serialize(const Targs &... args)
        {
            if(!serializing_)
                throw PulsarException("serialize called when we aren't serializing");
            if(unserializing_)
                throw std::logic_error("Serializing and unserializing at the same time");
            if(!oarchive_)
                throw std::logic_error("OArchive is not valid, but we are serializing");

            serialize_(args...);
        }


        /*! \brief Stop serialization
         * 
         * \throw pulsar::PulsarException if the object is
         *        unserializing or if it is not serializing to begin with.
         */
        void end_serialization(void)
        {
            if(serializing_ == false)
                throw PulsarException("Can't stop serializing - Not serializing");
            if(unserializing_ == true)
                throw PulsarException("Can't stop serializing - Currently unserializing");
            if(!oarchive_)
                throw std::logic_error("OArchive is not valid, but we are ending serialization");

            serializing_ = false;
            oarchive_.reset();
            Reset_();
        }



        /*! \brief Start unserialization of stored data
         * 
         * \throw pulsar::PulsarException if the object is
         *        already serializing or unserializing
         */
        void begin_unserialization(void)
        {
            if(serializing_ == true)
                throw PulsarException("Can't start unserializing - Currently serializing");
            if(unserializing_ == true)
                throw PulsarException("Can't start unserializing - Already unserializing");
            if(iarchive_)
                throw std::logic_error("IArchive is valid, but we aren't doing anything");

            unserializing_ = true;
            Reset_();

            iarchive_ = std::unique_ptr<InArchive>(new InArchive(stream_));
        }





        /*! \brief Extract data from the archive
         * 
         * \throw pulsar::PulsarException if we are serializing or we
         *        are not unserializing.
         */
        template<typename... Targs>
        void unserialize(Targs &... args)
        {
            if(!unserializing_)
                throw PulsarException("unserialize called when we aren't unserializing");
            if(serializing_)
                throw std::logic_error("Serializing and unserializing at the same time");
            if(!iarchive_)
                throw std::logic_error("IArchive is not valid, but we are unserializing");

            unserialize_(args...);
        }



        /*! \brief Extract data from the archive
         * 
         * Returns a single object. Useful for types without a public default constructor
         * 
         * \throw pulsar::PulsarException if we are serializing or we
         *        are not unserializing.
         */
        template<typename T>
        T unserialize_single(void)
        {
            T ret;
            unserialize(ret);
            return ret;
        }



        /*! \brief Stop unserialization
         * 
         * \throw pulsar::PulsarException if the object is
         *        serializing or if it is not unserializing to begin with.
         */
        void end_unserialization(void)
        {
            if(serializing_ == true)
                throw PulsarException("Can't stop unserializing - Currently serializing");
            if(unserializing_ == false)
                throw PulsarException("Can't stop unserializing - Not unserializing");
            if(!iarchive_)
                throw std::logic_error("IArchive is not valid, but we are ending unserialization");

            unserializing_ = false;;
            iarchive_.reset();
            Reset_();
        }


        /*! \brief Obtain the current size of stored data (in bytes)
         */ 
        size_t size(void)
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
        ByteArray to_byte_array(void)
        {
            const size_t s = size();

            begin_unserialization(); // perform checks and reset positions
            ByteArray ret(s);
            stream_.read(ret.data(), s);
            end_unserialization();
            return ret;
        }

        /*! \brief Create this archive from a byte array */
        void from_byte_array(const ByteArray & arr)
        {
            from_byte_array(arr.data(), arr.size());
        }

        /*! \brief Create this archive from a byte array */
        void from_byte_array(const char * arr, size_t size)
        {
            begin_serialization(); // perform checks and reset positions
            stream_.write(arr, size);
            end_serialization();
        }


    protected:
        template<typename ... Targs>
        StdStreamArchive(Targs &&... args)
            : stream_(std::forward<Targs>(args)...) { }

        STREAM stream_;
        bool serializing_ = false;
        bool unserializing_ = false;


    private:
        std::unique_ptr<InArchive> iarchive_;
        std::unique_ptr<OutArchive> oarchive_;

        void Reset_(void)
        {
            stream_.seekg(0, stream_.beg);
            stream_.seekp(0, stream_.beg);
        }

        void serialize_(void) { }

        template<typename T>
        void serialize_(const T & obj)
        {
            (*oarchive_)(obj);
        }


        template<typename T, typename... Targs>
        void serialize_(T & obj, Targs &... args)
        {
            serialize_(obj);
            serialize_(args...);
        }


        void unserialize_(void) { }

        template<typename T>
        void unserialize_(T & obj)
        {
            (*iarchive_)(obj);
        }


        template<typename T, typename... Targs>
        void unserialize_(T & obj, Targs &... args)
        {
            unserialize_(obj);
            unserialize_(args...);
        }

};

} // close namespace detail


/// Serialization of data to/from memory
class MemoryArchive : public detail::StdStreamArchive<std::stringstream>
{
    private:
        typedef detail::StdStreamArchive<std::stringstream> Base;

    public:
        MemoryArchive(void) 
            : detail::StdStreamArchive<std::stringstream>(std::stringstream::out |
                                                          std::stringstream::in |
                                                          std::stringstream::binary)
        { }

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
                throw PulsarException("Error opening file");
        }
};

} // close namespace pulsar



#endif

