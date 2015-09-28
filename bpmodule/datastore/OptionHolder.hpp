/*! \file
 *
 * \brief Storage of generic data (inner class) 
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_OPTIONHOLDER_HPP_
#define _GUARD_OPTIONHOLDER_HPP_

#include <vector>
#include <functional>
#include <boost/python.hpp>

#include "bpmodule/exception/OptionException.hpp"
#include "bpmodule/datastore/OptionBase.hpp"
#include "bpmodule/python_helper/Convert.hpp"
#include "bpmodule/output/Output.hpp"
#include "bpmodule/output/FormatStr.hpp"


//! \todo Split python stuff from header?

namespace bpmodule {
namespace datastore {
namespace detail {


////////////////////////////
// forward declarations
////////////////////////////
template<typename T>
class OptionHolder;


template<typename T>
void PrintOption_(const OptionHolder<T> & oph);

template<typename T>
void PrintOption_(const OptionHolder<std::vector<T>> & oph);
////////////////////////////
// end forward declarations
////////////////////////////



/*! \brief Holds the value and default for an option
 */
template<typename T>
class OptionHolder : public OptionBase
{
    public:
        //! A function that can validate an object of type T
        typedef std::function<bool(T)> ValidatorFunc;


        /*! \brief Constructs via pointers
         *
         * This object will take ownership of the value and def pointers.
         * The initial value and default will be validated (if given). 
         *
         * The value is not set on construction, only the default. Expert is set to false.
         *
         * \throw bpmodule::exception::OptionException
         *        If the initial value or default is invalid, or
         *        there is a default argument supplied for a 'required' option.
         *
         * \param [in] key The key of this option
         * \param [in] def The default value
         * \param [in] validator A validator function for this object
         *
         */
        OptionHolder(const std::string & key, T * def,
                     ValidatorFunc validator, bool required,
                     python_helper::PythonType pytype,
                     const std::string & help)
            : OptionBase(key, required, pytype, help),
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


        virtual void Print(void) const
        {
            PrintOption_(*this);
        }


        /////////////////////////////////////////
        // Python-related functions
        /////////////////////////////////////////
        virtual boost::python::object GetPy(void) const
        {
            if(!python_helper::TestConvertToPy(Get()))
                throw exception::OptionException("Cannot convert option value to python object", Key(),
                                                 "valuetype", Type());

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
         * If IsExpert(), a warning is printed. Otherwise, an exception is thrown.
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







////////////////////////////////////////////////////////
// Printing of options
////////////////////////////////////////////////////////
//! \todo Printing of a double?

template<typename T>
inline std::string OptToString_(const T & opt)
{
    return std::to_string(opt);
}

inline std::string OptToString_(const std::string & opt)
{
    return opt;
}


inline std::string OptToString_(const bool & opt)
{
    return (opt ? "True" : "False");
}


template<typename T>
inline void PrintOption_(const OptionHolder<T> & oph)
{
    std::string optline = FormatStr("    %|1$-20|      %|2$-20|      %|3$-20|      %|4$-20|     %|5$-10|       %6%\n",
                                    oph.Key(),                                                         // name/key
                                    python_helper::PythonTypeToStr(oph.PyType()),                      // type
                                    (oph.HasValue() ? OptToString_(oph.Get()) : "(none)"),             // value
                                    (oph.HasDefault() ? OptToString_(oph.GetDefault()) : "(none)"),    // default
                                    (oph.IsRequired() ? "True" : "False"),                             // required
                                    oph.Help());                                                       // help/description


    if(!oph.IsValid())
        output::Error(optline);
    
    if(!oph.IsDefault())
        output::Changed(optline);
    else
        output::Output(optline);
}



template<typename T>
inline void PrintOption_(const OptionHolder<std::vector<T>> & oph)
{
    size_t nrows = 0;

    std::string valstr, defstr;

    if(oph.HasValue())
    {
        auto valuevec = oph.Get();
        nrows = valuevec.size();

        if(valuevec.size() == 0)
            valstr = "(empty)";
        else
            valstr = OptToString_(valuevec[0]);
    }
    else
        valstr = "(none)";


    if(oph.HasDefault())
    {
        auto defvec = oph.GetDefault();
        nrows = std::max(nrows, defvec.size());

        if(defvec.size() == 0)
            defstr = "(empty)";
        else
            defstr = OptToString_(defvec[0]);
    }
    else
        defstr = "(none)";




    std::vector<std::string> optlines;
    optlines.push_back(FormatStr("    %|1$-20|      %|2$-20|      %|3$-20|      %|4$-20|     %|5$-10|       %6%\n",
                                 oph.Key(),                                                         // name/key
                                 python_helper::PythonTypeToStr(oph.PyType()),                      // type
                                 valstr,                                                            // value
                                 defstr,                                                            // default
                                 (oph.IsRequired() ? "True" : "False"),                             // required
                                 oph.Help()));                                                      // help/description


    // start at 1 since we did the first separately
    for(size_t i = 1; i < nrows; i++)
    {
        std::string valstr, defstr;

        if(oph.HasValue())
        {
            auto valuevec = oph.Get();
            if(valuevec.size() > i)
                valstr = OptToString_(valuevec[i]);
        }


        if(oph.HasDefault())
        {
            auto defvec = oph.GetDefault();
            if(defvec.size() > i)
                defstr = OptToString_(defvec[i]);
        }

        optlines.push_back(FormatStr("    %|1$-20|      %|2$-20|      %|3$-20|      %|4$-20|     %|5$-10|       %6%\n",
                                     "",              // name/key
                                     "",              // type
                                     valstr,          // value
                                     defstr,          // default
                                     "",              // required
                                     ""));            // help/description
        
    }

    for(const auto & it : optlines) 
    {
        if(!oph.IsValid())
            output::Error(it);
        else if(!oph.IsDefault())
            output::Changed(it);
        else
            output::Output(it);
    }


}




} //closing namespace detail
} //closing namespace datastore
} //closing namespace bpmodule


#endif
