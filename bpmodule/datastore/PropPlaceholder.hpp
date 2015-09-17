/*! \file
 *
 * \brief Storage of generic data (inner class) 
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_PROPPLACEHOLDER_HPP_
#define _GUARD_PROPPLACEHOLDER_HPP_

#include <memory>
#include <cstring>

#include "bpmodule/python_helper/BoostPython_fwd.hpp"

namespace bpmodule {
namespace datastore {
namespace detail {


/*! \brief An interface to a templated class that can hold anything
 *
 *  This allows for use in containers, etc.
 */
class PropPlaceholder
{
    public:
        PropPlaceholder(void) noexcept         = default;
        virtual ~PropPlaceholder() noexcept    = default;

        PropPlaceholder & operator=(const PropPlaceholder & rhs)  = delete;
        PropPlaceholder & operator=(const PropPlaceholder && rhs) = delete;
        PropPlaceholder(const PropPlaceholder & rhs)              = delete;
        PropPlaceholder(const PropPlaceholder && rhs)             = delete;



        /*! \brief Returns a string representing the type
         *
         * \exnothrow
         *
         * \return A string representing the type (obtained via typeid().name())
         */
        virtual const char * Type(void) const noexcept = 0;



        /*! \brief Determines if the contained type matches a given type
         *
         * \exnothrow
         *
         * \tparam U The type to compare to
         *
         * \return True if the contained object is of type U, false otherwise
         */ 
        template<typename U>
        bool IsType(void) const noexcept
        {
            return (strcmp(typeid(U).name(), Type()) == 0);
        }


        /*! \brief Return a copy of the data as a boost::python object
         *
         * \throw bpmodule::exception::PythonConvertException if the
         *        data could not be converted
         */ 
        virtual boost::python::object GetPy(void) const = 0;

};


//! A pointer to a generic object
typedef std::shared_ptr<PropPlaceholder> PropPlaceholderPtr;







} //closing namespace detail
} //closing namespace datastore
} //closing namespace bpmodule

#endif
