/*! \file
 *
 * \brief Storage of generic data (inner class) 
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_OPTIONHOLDER_HPP_
#define _GUARD_OPTIONHOLDER_HPP_

#include <functional>
#include <boost/python.hpp>

#include "bpmodule/datastore/OptionBase.hpp"
#include "bpmodule/exception/OptionException.hpp"
#include "bpmodule/python_helper/Convert.hpp"

//! \todo Split python stuff from header?

namespace bpmodule {
namespace datastore {
namespace detail {



/*! \brief Holds the value ad default for an option
 *
 * \todo Funnel ChangeValue through boost::python::object?....
 */
template<typename T>
class OptionHolder : public OptionBase
{
    public:
        //! A function that can validate an object of type T
        typedef std::function<bool(T)> ValidatorFunc;




        /*! \brief Constructs via copy
         *
         * \throw bpmodule::exception::OptionException
         *        If the initial value or default is invalid, or
         *        there is a default argument supplied for a 'required' option.
         *
         * \todo Should required even be passef? Default must be supplied here. Is
         *       this constructor even really needed?
         */
        OptionHolder(const T & value, const T & def,
                     ValidatorFunc validator, bool required, bool expert)
            : OptionBase(required, expert),
              value_(new T(value)),
              default_(new T(def)),
              validator_(validator)
        {
            // check the default and value with the validator
            if(!validator_(*value))
                throw exception::OptionException("Initial value is not valid", Type()); 
            if(!validator_(*def))
                throw exception::OptionException("Initial default is not valid", Type()); 
            if(required)
                throw exception::OptionException("Default value supplied for required value", Type());
        }




        /*! \brief Constructs via pointers
         *
         * This object will take ownership of the value and def pointers. 
         *
         * \throw bpmodule::exception::OptionException
         *        If the initial value or default is invalid, or
         *        there is a default argument supplied for a 'required' option.
         */
        OptionHolder(T * value, T * def,
                     ValidatorFunc validator, bool required, bool expert)
            : OptionBase(required, expert),
              value_(value),
              default_(def),
              validator_(validator)
        {
            // check the default and value with the validator
            if(value != nullptr && !validator_(*value))
                throw exception::OptionException("Initial value is not valid", Type()); 
            if(def != nullptr && !validator_(*def))
                throw exception::OptionException("Initial default is not valid", Type()); 
            if(def != nullptr && required)
                throw exception::OptionException("Default value supplied for required value", Type());
        }



        /*! \brief Copy constructor
         * 
         * Data will be deep copied
         */
        OptionHolder(const OptionHolder & oph)
            : OptionBase(oph),
              validator_(oph.validator_)
        {
            if(oph.value_)
                value_ = std::unique_ptr<T>(new T(*oph.value_));
            if(oph.default_)
                default_ = std::unique_ptr<T>(new T(*oph.default_));
        }



        OptionHolder(void)                                  = delete;
        OptionHolder(OptionHolder && oph)                   = delete;
        OptionHolder & operator=(const OptionHolder & oph)  = delete;
        OptionHolder & operator=(OptionHolder && oph)       = delete;
        virtual ~OptionHolder()                             = default;



        /*! \brief Change the stored value
         *
         * This object will take ownership of the pointer. 
         *
         * The new value will be validated.
         *
         * \throw bpmodule::exception::OptionException
         *        if the new value is invalid (and expert mode is off).
         *
         * \exstrong
         */
        void ChangeValue(T * value)
        {
            if(value != nullptr)
                Validate(*value);
            value_ = value;
        }



        /*! \brief Change the stored value
         *
         * This object will copy construct a new value
         *
         * The new value will be validated.
         *
         * \throw bpmodule::exception::OptionException
         *        if the new value is invalid (and expert mode is off).
         *
         * \exstrong
         *
         */
        void ChangeValue(const T & value)
        {
            // validate first
            ValidateOrThrow_(value);
            value_ = std::unique_ptr<T>(new T(value));
        }



        /*! \brief Get the current value
         *
         * If the value is not set, but a default exists, the default is returned.
         *
         * \throw bpmodule::exception::OptionException
         *        if the option does not have a value or a default
         *
         */
        const T & GetValue(void) const
        {
            if(value_)
                return *value_;
            else if(default_)
                return *default_;
            else
                throw exception::OptionException("Option does not have a value", Type());
        }




        /*! \brief Get the default value
         *
         * \throw bpmodule::exception::OptionException
         *        if the option does not have a default
         */
        const T & GetDefault(void) const
        {
            if(default_)
                return *default_;
            else
                throw exception::OptionException("Option does not have a default", Type());
        }




        /*! \brief Validate a value, but don't set it
         * 
         */
        bool Validate(const T & value) const
        {
            return validator_(value);
        }


        ////////////////////////////////////////
        // Virtual functions from OptionBase
        ////////////////////////////////////////

        virtual OptionBase * Clone(void) const
        {
            return new OptionHolder<T>(*this);
        }


        virtual constexpr const char * Type(void) const noexcept
        {
            return typeid(T).name();
        }

        
        virtual bool HasValue(void) const noexcept
        {
            return bool(value_) || HasDefault();
        }



        virtual bool HasDefault(void) const noexcept
        {
            return bool(default_);
        }



        virtual bool IsDefault(void) const
        {
            if(!value_ && default_)
                return true;
            else
                return value_ && default_ && (*value_ == *default_);
        }


        virtual void ResetToDefault(void)
        {
            if(!default_)
                throw exception::OptionException("Option does not have a default", Type());
            value_.reset();
        }



        /////////////////////////////////////////
        // Python-related functions
        /////////////////////////////////////////
        virtual boost::python::object GetValuePy(void) const
        {
            return python_helper::ConvertToPy(GetValue());
        }

        virtual void ChangeValuePy(const boost::python::object & obj)
        {
            ChangeValue(python_helper::ConvertToCpp<T>(obj));
        }

        virtual bool ValidatePy(const boost::python::object & obj) const
        {
            // throwing is ok
            const T tmp(python_helper::ConvertToCpp<T>(obj));
            return Validate(tmp);
        }


    private:
        std::unique_ptr<T> value_;
        std::unique_ptr<T> default_;
        ValidatorFunc validator_;


        /*! \brief Validate a potential value
         *
         * If IsExpert(), a warning is printed. Else, an exception is thrown.
         *
         * \throw bpmodule::exception::OptionException if the value is invalid
         *        and IsExpert() == false
         *
         * \todo print warning if expert_ == true
         */
        void ValidateOrThrow_(const T & value) const
        {
            if(!validator_(value))
            {
                if(!OptionBase::IsExpert())
                    throw exception::OptionException("Value is not valid for this option", Type());
            }
        }
};



/*! \brief Create an OptionHolder from a boost python object
 *
 * Returns as a pointer to the OptionBase class
 *
 * \throw bpmodule::exception::OptionException if there is a problem with
 *        the option (validation or invalid python types)
 *
 * \throw bpmodule::exception::PythonConvertException if there is a problem
 *        converting the data
 *
 */
OptionBasePtr OptionHolderFactory(const boost::python::object & obj);



} //closing namespace detail
} //closing namespace datastore
} //closing namespace bpmodule


#endif
