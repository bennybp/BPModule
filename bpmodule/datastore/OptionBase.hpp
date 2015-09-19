/*! \file
 *
 * \brief Storage of generic data (inner class) 
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_OPTIONBASE_HPP_
#define _GUARD_OPTIONBASE_HPP_

#include <memory>
#include <cstring>

#include "bpmodule/python_helper/BoostPython_fwd.hpp"

namespace bpmodule {
namespace datastore {
namespace detail {


class OptionBase
{
    public:
        OptionBase(void) noexcept         = default;
        virtual ~OptionBase() noexcept    = default;

        OptionBase & operator=(const OptionBase & rhs)  = delete;
        OptionBase & operator=(const OptionBase && rhs) = delete;
        OptionBase(const OptionBase & rhs)              = delete;
        OptionBase(const OptionBase && rhs)             = delete;


        virtual OptionBase * Clone(void) const = 0;

        /*! \brief Returns a string representing the type of the option stored
         *
         * \exnothrow
         *
         * \return A string representing the type (obtained via typeid().name())
         */
        virtual const char * Type(void) const noexcept = 0;



        virtual bool HasValue(void) const = 0;
        virtual bool HasDefault(void) const = 0;
        virtual bool IsRequired(void) const = 0;

        bool Has(void) const
        {
            return (HasValue() || HasDefault());
        }



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


        /*! \brief Return a copy of the value as a boost::python object
         *
         * \throw bpmodule::exception::PythonConvertException if the
         *        data could not be converted
         *
         * \throw bpmodule::exception::OptionException if the
         *        value does not exist
         */ 
        //virtual boost::python::object GetValuePy(void) const = 0;


    

};


typedef std::shared_ptr<OptionBase> OptionBasePtr;







} //closing namespace detail
} //closing namespace datastore
} //closing namespace bpmodule

#endif
