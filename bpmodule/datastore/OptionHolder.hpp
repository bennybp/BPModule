/*! \file
 *
 * \brief Storage of generic data (inner class) 
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_OPTIONHOLDER_HPP_
#define _GUARD_OPTIONHOLDER_HPP_

#include <functional>
#include <boost/python.hpp>

#include "bpmodule/exception/OptionException.hpp"
#include "bpmodule/datastore/OptionBase.hpp"
#include "bpmodule/python_helper/Convert.hpp"
#include "bpmodule/output/Output.hpp"


//! \todo Split python stuff from header?

namespace bpmodule {
namespace datastore {
namespace detail {



/*! \brief Holds the value and default for an option
 */
template<typename T>
class OptionHolder : public OptionBase
{
    public:
        //! A function that can validate an object of type T
        typedef std::function<bool(T)> ValidatorFunc;




        /*! \brief Constructs via copy
         *
         * Since the default argument must be given, required is set to false
         * The initial value and default will be validated. 
         *
         * The value is not set on construction, only the default.
         *
         * \throw bpmodule::exception::OptionException If the initial value or default is invalid
         */
        OptionHolder(const std::string & key, const T & def,
                     ValidatorFunc validator, bool expert)
            : OptionBase(key, false, expert),
              default_(new T(def)),
              validator_(validator)
        {
            // check the default and value with the validator
            ValidateOrThrow_(*default_, "initial default");
        }




        /*! \brief Constructs via pointers
         *
         * This object will take ownership of the value and def pointers.
         * The initial value and default will be validated (if given). 
         *
         * The value is not set on construction, only the default.
         *
         * \throw bpmodule::exception::OptionException
         *        If the initial value or default is invalid, or
         *        there is a default argument supplied for a 'required' option.
         */
        OptionHolder(const std::string & key, T * def,
                     ValidatorFunc validator, bool required, bool expert)
            : OptionBase(key, required, expert),
              default_(def),
              validator_(validator)
        {
            // check the default using the validator
            if(def != nullptr)
                ValidateOrThrow_(*default_, "initial default");
            if(def != nullptr && required)
                throw exception::OptionException("Default value supplied for required option", Key()); 
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
         * The new value will be validated.
         *
         * \throw bpmodule::exception::OptionException
         *        If the new value is invalid (and expert mode is off).
         *
         * \exstrong
         */
        void Change(T * value)
        {
            if(value != nullptr)
            {
                ValidateOrThrow_(*value);
                value_ = value;
            }
            else
                value_.reset();
        }



        /*! \brief Change the stored value
         *
         * This object will copy construct a new value
         *
         * The new value will be validated.
         *
         * \throw bpmodule::exception::OptionException
         *        If the new value is invalid (and expert mode is off).
         *
         * \exstrong
         */
        void Change(const T & value)
        {
            ValidateOrThrow_(value, "new value");
            value_ = std::unique_ptr<T>(new T(value));
        }



        /*! \brief Get the current value
         *
         * If the value is not set, but a default exists, the default is returned.
         *
         * \throw bpmodule::exception::OptionException
         *        If the option does not have a value or a default
         */
        const T & Get(void) const
        {
            if(value_)
                return *value_;
            else if(default_)
                return *default_;
            else
                throw exception::OptionException("Option does not have a value", Key());
        }




        /*! \brief Get the default value
         *
         * \throw bpmodule::exception::OptionException
         *        If the option does not have a default
         */
        const T & GetDefault(void) const
        {
            if(default_)
                return *default_;
            else
                throw exception::OptionException("Option does not have a default", Key());
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
            return bool(value_) || bool(default_);
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
            if(!python_helper::TestConvertToPy(Get()))
                throw exception::OptionException("Cannot convert option value to python object", Key(), "valuetype", Type());

            return python_helper::ConvertToPy(Get());
        }


        virtual void ChangePy(const boost::python::object & obj)
        {
            if(!python_helper::TestConvertToCpp<T>(obj))
                throw exception::OptionException("Cannot convert python object to option value", Key(),
                                                 "valuetype", Type(),
                                                 "pythontype", python_helper::GetPyClass(obj));

            // will validate inside Change()
            Change(python_helper::ConvertToCpp<T>(obj));
        }



    private:
        std::unique_ptr<T> value_;
        std::unique_ptr<T> default_;
        ValidatorFunc validator_;


        /*! \brief Validate a potential value
         *
         * If IsExpert(), a warning is printed. Else, an exception is thrown.
         *
         * \throw bpmodule::exception::OptionException
         *        If the value is invalid and IsExpert() == false
         *
         * \param [in] value The value to validate
         * \param [in] desc Short description of what is being validated
         */
        void ValidateOrThrow_(const T & value, const std::string & desc) const
        {
            if(!validator_(value))
            {
                //! \todo add exception info from validator?
                if(!OptionBase::IsExpert())
                    throw exception::OptionException("Value is not valid for this option", Key());
                else
                    output::Warning("Value for option %1% \"%2\" is invalid. Ignoring\n", desc, Key());
            }
        }
};



/*! \brief Create an OptionHolder from a boost python object
 *
 * Returns as a pointer to the OptionBase class
 *
 * \throw bpmodule::exception::OptionException
 *        If there is a problem with the option (validation or python conversion problems)
 */
OptionBasePtr OptionHolderFactory(const std::string & key, const boost::python::object & obj);



} //closing namespace detail
} //closing namespace datastore
} //closing namespace bpmodule


#endif
