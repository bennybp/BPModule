/*! \file
 *
 * \brief Storage of generic data (inner class)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_OPTIONHOLDER_HPP_
#define _GUARD_OPTIONHOLDER_HPP_

#include <functional>

#include "bpmodule/python/Pybind11_functional.hpp"
#include "bpmodule/exception/OptionException.hpp"
#include "bpmodule/python/Convert.hpp"


namespace bpmodule {
namespace datastore {


typedef std::vector<std::string> OptionIssues;


/*! \brief Holds the value and default for an option
 *
 */
class OptionHolder
{
    public:
        //! A function that can validate an object of type T
        typedef std::function<OptionIssues (const OptionHolder &)> ValidatorFunc;


        /*! \brief Constructs via pointers
         *
         * This object will take ownership of the value and def pointers.
         * The default value will be validated (if given).
         *
         * The value is not set on construction, only the default
         *
         * \throw bpmodule::exception::OptionException
         *        If the default value is invalid, or
         *        there is a default argument supplied for a 'required' option.
         *
         * \throw bpmodule::exception::PythonCallException
         *       If there is a problem calling the validation function
         *
         * \param [in] key The key of this option
         * \param [in] def The default value
         * \param [in] validator A validator function for this object
         * \param [in] required True if this option is required
         * \param [in] pytype The python type of this option
         * \param [in] help A help string for this option
         *
         */
        OptionHolder(const std::string & key, const pybind11::object & def,
                     ValidatorFunc validator, bool required,
                     const std::string & pytype,
                     const std::string & help);


        OptionHolder(const OptionHolder & oph);

        OptionHolder(void)                                  = delete;
        OptionHolder(OptionHolder && oph)                   = default;
        OptionHolder & operator=(const OptionHolder & oph)  = delete;
        OptionHolder & operator=(OptionHolder && oph)       = default;
        ~OptionHolder()                                     = default;



        /*! \brief Change the stored value
         *
         * This object will copy construct a new value
         *
         * \exstrong
         */
        template<typename T>
        void Change(const T & value)
        {
            value_ = python::ConvertToPy(value);
        }



        /*! \brief Get the current value
         *
         * If the value is not set, but a default exists, the default is returned.
         *
         * \throw bpmodule::exception::OptionException
         *        If the option does not have a value or a default
         */
        template<typename T>
        T Get(void) const
        {
            if(value_.ptr() != nullptr)
                return python::ConvertToCpp<T>(value_);
            else if(default_.ptr() != nullptr)
                return python::ConvertToCpp<T>(default_);
            else
                throw exception::OptionException("Option does not have a value", Key());
        }



        /*! \brief Get the default value
         *
         * \throw bpmodule::exception::OptionException
         *        If the option does not have a default
         */
        template<typename T>
        T GetDefault(void) const
        {
            if(default_.ptr() != nullptr)
                return python::ConvertToCpp<T>(default_);
            else
                throw exception::OptionException("Option does not have a default", Key());
        }



        /*! \brief Get the python type of this option
         *
         * \exnothrow
         */
        const std::string & PyType(void) const noexcept;



        /*! \brief \todo
         */
        bool HasValue(void) const noexcept;



        /*! \brief \todo
         */
        bool HasDefault(void) const noexcept;


        /*! \brief \todo
         */
        bool IsDefault(void) const;



        /*! \brief \todo
         */
        void ResetToDefault(void);



        /*! \brief Get the help string for this option
         *
         * \exnothrow
         */
        const std::string & Help(void) const noexcept;



        /*! \brief Test if this option has issues
         *
         * \throw bpmodule::exception::PythonCallException if there is a problem
         *        with the validation function
         */
        bool HasIssues(void) const;



        /*! \brief \todo
         */
        void Print(void) const;



        /*! \brief \todo
         */
        OptionIssues GetIssues(void) const;



        /*! \brief \todo
         */
        bool Compare(const OptionHolder & rhs) const;



        /*! \brief Return the key of this option
         *
         * \exnothrow
         */
        const std::string & Key(void) const noexcept;



        /*! \brief Check if this options is required
         *
         * \exnothrow
         */
        bool IsRequired(void) const noexcept;



        /*! \brief Check to see if this object is set if required
         *
         * \return True if there is a value or a default, or if this
         *         option is not required
         */
        bool IsSetIfRequired(void) const;



    private:
        //! The key of this option
        const std::string key_;

        //! Is this option required
        const bool required_;

        //! The python type of this option
        const std::string pytype_;

        //! The help string for this option
        const std::string help_;

        //! A set value for the option
        pybind11::object value_;

        //! The default for the option
        pybind11::object default_;

        //! Validation function object
        ValidatorFunc validator_;

};





} //closing namespace datastore
} //closing namespace bpmodule


#endif
