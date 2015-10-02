/*! \file
 *
 * \brief Storage of generic data (inner class) 
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_OPTIONHOLDER_HPP_
#define _GUARD_OPTIONHOLDER_HPP_

#include <functional>

#include "bpmodule/options/OptionBase.hpp"


namespace bpmodule {
namespace options {
namespace detail {



/*! \brief Holds the value and default for an option
 *
 * \note This is a template class, but definitions are in the
 *       cpp file. This class is then defined for the
 *       allowed option types.
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
         * \param [in] required True if this option is required
         * \param [in] pytype The python type of this option
         * \param [in] help A help string for this option
         *
         */
        OptionHolder(const std::string & key, T * def,
                     ValidatorFunc validator, bool required,
                     python_helper::PythonType pytype,
                     const std::string & help);



        /*! \brief Copy constructor
         * 
         * Data will be deep copied
         */
        OptionHolder(const OptionHolder & oph);




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
        void Change(const T & value);



        /*! \brief Get the current value
         *
         * If the value is not set, but a default exists, the default is returned.
         *
         * \throw bpmodule::exception::OptionException
         *        If the option does not have a value or a default
         */
        const T & Get(void) const;




        /*! \brief Get the default value
         *
         * \throw bpmodule::exception::OptionException
         *        If the option does not have a default
         */
        const T & GetDefault(void) const;



        ////////////////////////////////////////
        // Virtual functions from OptionBase
        ////////////////////////////////////////

        virtual OptionBase * Clone(void) const;

        virtual constexpr const char * Type(void) const noexcept;

        virtual bool HasValue(void) const noexcept;

        virtual bool HasDefault(void) const noexcept;

        virtual bool IsDefault(void) const;

        virtual void ResetToDefault(void) noexcept;

        virtual void Print(void) const;


        /////////////////////////////////////////
        // Python-related functions
        /////////////////////////////////////////
        virtual boost::python::object GetPy(void) const;

        virtual void ChangePy(const boost::python::object & obj);


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
        void ValidateOrThrow_(const T & value, const std::string & desc) const;
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
} //closing namespace options
} //closing namespace bpmodule


#endif
