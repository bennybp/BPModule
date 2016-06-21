/*! \file
 *
 * \brief Storage of generic data (inner class) 
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef PULSAR_GUARD_DATASTORE__GENERICHOLDER_HPP_
#define PULSAR_GUARD_DATASTORE__GENERICHOLDER_HPP_

#include "pulsar/util/Mangle.hpp"
#include "pulsar/datastore/GenericBase.hpp"


namespace pulsar{
namespace datastore {
namespace detail {



/*! \brief A container that can hold anything
 *
 * \tparam T The type of the data this object is holding
 */ 
template<typename T>
class GenericHolder : public GenericBase
{
    public:
        /*! \brief Construct via copying a data object
         * 
         * Will invoke copy constructor for type T
         *
         * \throwno Throws an exception only if the move
         *          constructor for T throws an exception
         *
         *  \param [in] m The object to copy
         */
        GenericHolder(const T & m)
            : obj(m)
        { }


        /*! \brief Construct via moving a data object
         * 
         * Will invoke move constructor for type T
         *
         * \throwno Throws an exception only if the move
         *          constructor for T throws an exception
         *
         * \param [in] m The object to move
         */
        GenericHolder(T && m)
            : obj(std::move(m))
        { }


        // no other constructors, etc
        GenericHolder(void)                                = delete;
        GenericHolder(const GenericHolder & oph)              = delete;
        GenericHolder(GenericHolder && oph)                   = delete;
        GenericHolder & operator=(const GenericHolder & oph)  = delete;
        GenericHolder & operator=(GenericHolder && oph)       = delete;
        virtual ~GenericHolder()                           = default;


        /*! Return a reference to the underlying data
         *
         * \exnothrow
         *
         * \return A reference to the underlying data
         */ 
        T & get_ref(void) noexcept
        {
            return obj;
        }


        /*! Return a const reference to the underlying data
         *
         * \exnothrow
         *
         * \return A const reference to the underlying data
         */ 
        const T & get_ref(void) const noexcept
        {
            return obj;
        }


        ////////////////////////////////////////
        // Virtual functions from GenericBase
        ////////////////////////////////////////
        virtual const char * type(void) const noexcept
        {
            return typeid(T).name();
        }

        virtual const std::type_info & type_info(void) const noexcept
        {
            return typeid(T);
        }

        
        virtual std::string demangled_type(void) const
        {
            return util::demangle_cpp_or_py_type(obj);
        }


    private:
        //! The actual data
        T obj;
};



} //closing namespace detail
} //closing namespace datastore
} //closing namespace pulsar


#endif
