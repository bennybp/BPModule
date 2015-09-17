/*! \file
 *
 * \brief Storage of generic data (inner class) 
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_PROPHOLDER_HPP_
#define _GUARD_PROPHOLDER_HPP_

#include "bpmodule/datastore/PropPlaceholder.hpp"

namespace bpmodule {
namespace datastore {
namespace detail {



/*! \brief A container that can hold anything
 *
 * \tparam T The type of the data this object is holding
 */ 
template<typename T>
class PropHolder : public PropPlaceholder
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
        PropHolder(const T & m);


        /*! \brief Construct via moving a data object
         * 
         * Will invoke move constructor for type T
         *
         * \throwno Throws an exception only if the move
         *          constructor for T throws an exception
         *
         * \param [in] m The object to move
         */
        PropHolder(T && m);


        // no other constructors, etc
        PropHolder(void)                                = delete;
        PropHolder(const PropHolder & oph)              = delete;
        PropHolder(PropHolder && oph)                   = delete;
        PropHolder & operator=(const PropHolder & oph)  = delete;
        PropHolder & operator=(PropHolder && oph)       = delete;
        virtual ~PropHolder()                           = default;


        /*! Return a reference to the underlying data
         *
         * \exnothrow
         *
         * \return A reference to the underlying data
         */ 
        T & GetRef(void) noexcept;


        /*! Return a const reference to the underlying data
         *
         * \exnothrow
         *
         * \return A const reference to the underlying data
         */ 
        const T & GetRef(void) const noexcept;


        virtual constexpr const char * Type(void) const noexcept
        {
            return typeid(T).name();
        }


        virtual boost::python::object GetPy(void) const;

    private:
        //! The actual data
        T obj;
};



/*! \brief Create a detail::PropPlaceHolder from python object
 *
 * This copies the data from the object.
 * 
 * \throw bpmodule::exception::PythonConvertException if there is a problem with a conversion
 *
 * \param [in] obj A python object containing data to copy
 */ 
PropPlaceholderPtr PropHolderFactory(const boost::python::object & obj);



} //closing namespace detail
} //closing namespace datastore
} //closing namespace bpmodule


#endif
