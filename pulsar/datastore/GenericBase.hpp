/*! \file
 *
 * \brief Storage of generic data (inner class) 
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef PULSAR_GUARD_DATASTORE__GENERICBASE_HPP_
#define PULSAR_GUARD_DATASTORE__GENERICBASE_HPP_

#include "pulsar/util/Serialization_fwd.hpp"

#include <bphash/Hash.hpp>

#include <memory>

namespace pulsar{
namespace datastore {
namespace detail {


/* Developer note
 *
 * Why not just have serialize functions and let cereal handle the
 * polymorphic serialization?
 *
 * Answer: It would require registering every type of GenericHolder<T>,
 * including any types held in the modules. That's a pain, so I work around
 * it a bit
 */


/*! \brief An interface to a templated class that can hold anything
 *
 *  This allows for use in containers, etc.
 */
class GenericBase
{
    public:
        GenericBase(void) noexcept         = default;
        virtual ~GenericBase() noexcept    = default;

        GenericBase & operator=(const GenericBase & rhs)  = delete;
        GenericBase & operator=(const GenericBase && rhs) = delete;
        GenericBase(const GenericBase & rhs)              = delete;
        GenericBase(const GenericBase && rhs)             = delete;



        ///////////////////////////////////
        // Virtual functions
        ///////////////////////////////////
        /*! \brief Returns a string representing the type
         *
         * \exnothrow
         *
         * \return A string representing the type (obtained via typeid().name())
         */
        virtual const char * type(void) const noexcept = 0;


        /*! \brief Returns a string representing the demangled type of the stored object
         *
         * For a C++ object, will return the demangled C++ type. For a python
         * object, will return its class string.
         *
         * \return A string representing the type
         */
        virtual std::string demangled_type(void) const = 0;

        /*! \brief Check if the data stored in this object is serializable */
        virtual bool is_serializable(void) const noexcept = 0;

        /*! \brief Check if the data stored in this object is hashable */
        virtual bool is_hashable(void) const noexcept = 0;

        /*! \brief Serialize the data as a byte array */
        virtual ByteArray to_byte_array(void) const = 0;

        /*! \brief Unserialize a byte array */
        virtual void from_byte_array(const ByteArray & arr) = 0;

        /*! \brief Obtain the hash of the data */
        virtual bphash::HashValue my_hash(void) const = 0;
};




} //closing namespace detail
} //closing namespace datastore
} //closing namespace pulsar

#endif
