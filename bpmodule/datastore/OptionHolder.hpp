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


namespace bpmodule {
namespace datastore {
namespace detail {



template<typename T>
class OptionHolder : public OptionBase
{
    public:
        typedef std::function<bool(T)> ValidatorFunc;

        OptionHolder(const T & value, const T & def, ValidatorFunc validator, bool required, bool expert)
            : value_(new T(value)),
              default_(new T(def)),
              validator_(validator),
              required_(required),
              expert_(expert)
        {
            // check the default and value with the validator
            if(!validator_(value))
                throw exception::OptionException("Initial value is not valid", Type()); 
            if(!validator_(def))
                throw exception::OptionException("Initial default is not valid", Type()); 
        }


        OptionHolder(const OptionHolder & oph)
        {
            validator_ = oph.validator_;
            required_ = oph.required_;
            expert_ = oph.expert_;

            if(oph.value_)
                value_(new T(*oph.value_));
            if(oph.default_)
                default_(new T(*oph.default_));
        }


        OptionHolder(OptionHolder && oph)                   = default;

        OptionHolder(void)                                  = delete;
        OptionHolder & operator=(const OptionHolder & oph)  = delete;
        OptionHolder & operator=(OptionHolder && oph)       = delete;
        virtual ~OptionHolder()                             = default;


        virtual OptionBase * Clone(void) const
        {
            return new OptionHolder<T>(*this);
        }


        const T & GetValue(void) const
        {
            if(value_)
                return *value_;
            else if(default_)
                return *default_;
            else
                throw exception::OptionException("Option does not have a value", Type());
        }


        const T & GetDefault(void) const
        {
            if(default_)
                return *default_;
            else
                throw exception::OptionException("Option does not have a default", Type());
        }
        

        virtual bool HasValue(void) const
        {
            return value_;
        }

        virtual bool HasDefault(void) const
        {
            return default_;
        }

        // should a required value have a default?
        virtual bool IsRequired(void) const
        {
            return required_ && !HasDefault();
        }


        virtual bool IsDefault(void) const
        {
            return value_ && default_ && (*value_ == *default_);
        }

        void ChangeValue(const T & value)
        {
            // validate first
            Validate_(value);
            value_ = std::unique_ptr<T>(new T(value));
        }


        // may throw
        // default should already have been validated
        virtual void ResetToDefault(void)
        {
            ChangeValue(GetDefault());
        }


        virtual constexpr const char * Type(void) const noexcept
        {
            return typeid(T).name();
        }


        virtual boost::python::object GetPy(void) const;

    private:
        std::unique_ptr<T> value_;
        std::unique_ptr<T> default_;
        ValidatorFunc validator_;
        bool required_;
        bool expert_;


        void Validate_(const T & value) const
        {
            // validate first
            if(!validator_(value))
            {
                //! \todo print warning
                if(!expert_)
                    throw exception::OptionException("Value is not valid for this option", Type());
            }
        }
};



//OptionBasePtr OptionHolderFactory(const boost::python::tuple & tup);
//{
//    
//}



} //closing namespace detail
} //closing namespace datastore
} //closing namespace bpmodule


#endif
