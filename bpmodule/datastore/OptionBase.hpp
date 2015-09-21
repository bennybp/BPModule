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
        OptionBase(bool required, bool expert) noexcept
            : required_(required), expert_(expert)
        { }

        virtual ~OptionBase() noexcept    = default;

        OptionBase & operator=(const OptionBase & rhs)  = delete;
        OptionBase & operator=(OptionBase && rhs)       = delete;
        OptionBase(const OptionBase & rhs)              = default;
        OptionBase(const OptionBase && rhs)             = delete;



        ///////////////////////////////////
        // Virtual functions
        ///////////////////////////////////
        
        /*! \brief Create a clone of this object
         */
        virtual OptionBase * Clone(void) const = 0;



        /*! \brief Returns a string representing the type of the option stored
         *
         * \exnothrow
         *
         * \return A string representing the type (obtained via typeid().name())
         */
        virtual const char * Type(void) const noexcept = 0;



        /*! \brief Check if this option has a value
         * 
         * \return True if this option has a value or a default
         */
        virtual bool HasValue(void) const noexcept = 0;


        /*! \brief Check if this option has a default
         * 
         * \return True if this option has a default
         */
        virtual bool HasDefault(void) const noexcept = 0;



        /*! \brief Check if this option is set to the default
         */ 
        virtual bool IsDefault(void) const noexcept = 0;



        /*! \brief Set the option to its default
         *  \throw bpmodule::exception::OptionException
         *         if the option doesn't have a default
         */ 
        virtual void ResetToDefault(void) = 0;


        /////////////////////////////////////////
        // Python-related functions
        /////////////////////////////////////////
        /*! \brief Return a copy of the value as a boost::python object
         *
         * \throw bpmodule::exception::PythonConvertException if the
         *        data could not be converted
         *
         * \throw bpmodule::exception::OptionException if the
         *        value does not exist
         */ 
        virtual boost::python::object GetPyValue(void) const = 0;



        /*! \brief Change the value with a boost python object
         */  
        virtual void ChangeValue(const boost::python::object & obj) = 0;


        /*! \brief Validate a value, but don't set it
         * 
         * \throw exception::PythonConvertException if the
         *        python object could not be converted
         */
        virtual bool Validate(const boost::python::object & obj) const = 0;


        ///////////////////////////////////
        // Base functions
        ///////////////////////////////////
        /*! \brief Check if this options is required
         */ 
        bool IsRequired(void) const noexcept
        {
            return required_;
        }



        /*! \brief See if expert mode is enabled for this object
         *
         */
        bool IsExpert(void) const noexcept
        {
            return expert_;
        }



        /*! \brief Check to see if this object is valid
         * 
         * \return True if there is a value or a default, or if this
         *         option is not required
         */
        bool Valid(void) const
        {
            return HasValue() || HasDefault() || !IsRequired();
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


    private:
        bool required_;
        bool expert_;


    

};


typedef std::shared_ptr<OptionBase> OptionBasePtr;







} //closing namespace detail
} //closing namespace datastore
} //closing namespace bpmodule

#endif
