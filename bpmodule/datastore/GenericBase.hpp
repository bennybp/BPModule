/*! \file
 *
 * \brief Storage of generic data (inner class) 
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_GENERICBASE_HPP_
#define _GUARD_GENERICBASE_HPP_

#include <memory>

#include "bpmodule/python_helper/BoostPython_fwd.hpp"

namespace bpmodule {
namespace datastore {
namespace detail {


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
        virtual const char * Type(void) const noexcept = 0;



        /*! \brief Returns the std::type_info for the stored object
         *
         * \exnothrow
         *
         * \return A std::type_info structure representing the stored type
         */
        virtual const std::type_info & TypeInfo(void) const noexcept = 0;



        /*! \brief Returns a string representing the demangled type of the stored object
         *
         * \return A string representing the type (obtained via typeid().name(), but demangled)
         */
        virtual std::string DemangledType(void) const = 0;



        ///////////////////////////////////
        // Base functions
        ///////////////////////////////////
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
            return typeid(U) == TypeInfo();
        }


        ///////////////////////////////////
        // Python functions
        ///////////////////////////////////
        /*! \brief Return a copy of the data as a boost::python object
         *
         * \throw bpmodule::exception::PythonConvertException if the
         *        data could not be converted
         */ 
        virtual boost::python::object GetPy(void) const = 0;

};


//! A pointer to a generic object
typedef std::shared_ptr<GenericBase> GenericBasePtr;







} //closing namespace detail
} //closing namespace datastore
} //closing namespace bpmodule

#endif
