/*! \file
 *
 * \brief Storage of generic data (inner class) 
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef BPMODULE_GUARD_DATASTORE__GENERICBASE_HPP_
#define BPMODULE_GUARD_DATASTORE__GENERICBASE_HPP_

#include <memory>

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
         * For a C++ object, will return the demangled C++ type. For a python
         * object, will return its class string.
         *
         * \return A string representing the type
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

};


//! A pointer to a generic object
typedef std::shared_ptr<GenericBase> GenericBasePtr;







} //closing namespace detail
} //closing namespace datastore
} //closing namespace bpmodule

#endif
