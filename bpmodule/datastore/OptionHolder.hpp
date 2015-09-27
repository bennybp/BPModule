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
        OptionHolder(const std::string & key, const T & value, const T & def,
                     ValidatorFunc validator, bool required, bool expert)
            : OptionBase(key, required, expert),
              value_(new T(value)),
              default_(new T(def)),
              validator_(validator)
        {
            // check the default and value with the validator
            if(!validator_(*value))
                throw exception::GeneralException("Initial value is not valid",
                                                  "optionkey", Key(),
                                                  "type", Type()); 
            if(!validator_(*def))
                throw exception::GeneralException("Initial default is not valid",
                                                  "optionkey", Key(),
                                                  "type", Type()); 
            if(required)
                throw exception::GeneralException("Default value supplied for required option",
                                                  "optionkey", Key(),
                                                  "type", Type()); 
        }




        /*! \brief Constructs via pointers
         *
         * This object will take ownership of the value and def pointers. 
         *
         * \throw bpmodule::exception::OptionException
         *        If the initial value or default is invalid, or
         *        there is a default argument supplied for a 'required' option.
         */
        OptionHolder(const std::string & key, T * value, T * def,
                     ValidatorFunc validator, bool required, bool expert)
            : OptionBase(key, required, expert),
              value_(value),
              default_(def),
              validator_(validator)
        {
            // check the default and value with the validator
            if(value != nullptr && !validator_(*value))
                throw exception::GeneralException("Initial value is not valid",
                                                  "optionkey", Key(),
                                                  "type", Type()); 
            if(def != nullptr && !validator_(*def))
                throw exception::GeneralException("Initial default is not valid",
                                                  "optionkey", Key(),
                                                  "type", Type()); 
            if(def != nullptr && required)
                throw exception::GeneralException("Default value supplied for required option",
                                                  "optionkey", Key(),
                                                  "type", Type()); 
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
        void Change(T * value)
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
        void Change(const T & value)
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
        const T & Get(void) const
        {
            if(value_)
                return *value_;
            else if(default_)
                return *default_;
            else
                throw exception::GeneralException("Option does not have a value",
                                                  "optionkey", Key(),
                                                  "type", Type()); 
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
                throw exception::GeneralException("Option does not have a default",
                                                  "optionkey", Key(),
                                                  "type", Type()); 
        }



        /*! \brief Test validation of a value, but don't set it
         * 
         */
        bool Test(const T & value) const
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


        virtual void ResetToDefault(void) noexcept
        {
            value_.reset();
        }



        /////////////////////////////////////////
        // Python-related functions
        /////////////////////////////////////////
        virtual boost::python::object GetPy(void) const
        {
            return python_helper::ConvertToPy(Get());
        }

        virtual void ChangePy(const boost::python::object & obj)
        {
            Change(python_helper::ConvertToCpp<T>(obj));
        }

        virtual bool TestPy(const boost::python::object & obj) const
        {
            if(!python_helper::TestConvertToCpp<T>(obj))
                return false;

            const T tmp(python_helper::ConvertToCpp<T>(obj));
            return Test(tmp);
        }


        virtual bool TestConvertPy(const boost::python::object & obj) const
        {
            return python_helper::TestConvertToCpp<T>(obj);
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
                    throw exception::GeneralException("Value is not valid for this option",
                                                      "optionkey", Key(),
                                                      "type", Type());
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
OptionBasePtr OptionHolderFactory(const std::string & key, const boost::python::object & obj);



} //closing namespace detail
} //closing namespace datastore
} //closing namespace bpmodule


#endif
