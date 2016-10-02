/*! \file
 *
 * \brief Storage of generic data (inner class) 
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#pragma once

//! \todo can I forward declare GenericHolder?
#include "pulsar/datastore/GenericHolder.hpp"

namespace pulsar {
namespace datastore {
namespace detail {


template<>
class GenericHolder<pybind11::object> : public GenericBase
{
    public:
        GenericHolder(const pybind11::object & m);

        // no other constructors, etc
        GenericHolder(void)                                   = delete;
        GenericHolder(const GenericHolder & oph)              = delete;
        GenericHolder(GenericHolder && oph)                   = delete;
        GenericHolder & operator=(const GenericHolder & oph)  = delete;
        GenericHolder & operator=(GenericHolder && oph)       = delete;
        virtual ~GenericHolder()                              = default;


        /*! Return a const reference to the underlying data
         *
         * \return A const reference to the underlying data
         */ 
        std::shared_ptr<const pybind11::object> get(void) const;


        ////////////////////////////////////////
        // Virtual functions from GenericBase
        ////////////////////////////////////////
        virtual const char * type(void) const noexcept;

        virtual std::string demangled_type(void) const;

        ///////////////////////////////
        // Serialization
        ///////////////////////////////

        virtual bool is_serializable(void) const noexcept;

        virtual ByteArray to_byte_array(void) const;

        virtual void from_byte_array(const ByteArray & arr);

        ///////////////////////////////
        // Hashing
        ///////////////////////////////
        virtual bool is_hashable(void) const noexcept;

        virtual bphash::HashValue my_hash(void) const;

    protected:
        //! The actual data
        // NOTE: We have this as a sherd pointer to make the
        // return type of get() be the same for all types
        // of objects (python or otherwise). Dereferencing to the
        // actual python object takes place in export.cpp
        std::shared_ptr<pybind11::object> obj;
};




} //closing namespace detail
} //closing namespace datastore
} //closing namespace pulsar

