/*! \file
 *
 * \brief Storage of options data (base class) (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_DATASTORE__OPTIONBASE_HPP_
#define PULSAR_GUARD_DATASTORE__OPTIONBASE_HPP_

#include <ostream>

#include <pybind11/pybind11.h>
#include "pulsar/util/Serialization.hpp"

#include "bphash/types/string.hpp" // Includes Hasher

namespace pulsar {

//! A collection of problems with an option
typedef std::vector<std::string> OptionIssues;


// forward declare
enum class OptionType;




namespace detail {


// forward declare class
class OptionBase;

//! A pointer to an option
typedef std::unique_ptr<OptionBase> OptionBasePtr;



/*! \brief Base class of OptionHolder
 *
 * Defines the interface for generic stored options
 */
class OptionBase
{
    public:
        /*! \brief Constructor
         *
         * \param [in] key The key of this option
         * \param [in] required True if this is a required option
         * \param [in] help A help string for this option
         */
        OptionBase(const std::string & key, bool required, const std::string & help)
            : key_(key), required_(required), help_(help)
        {  }


        /// \copydocs OptionBase(const std::string &, bool, const std::string &)
        OptionBase(std::string && key, bool required, std::string && help)
            : key_(std::move(key)), required_(required), help_(std::move(help))
        { }


        virtual ~OptionBase() noexcept              = default;

        // Don't allow copying / assigning
        OptionBase & operator=(const OptionBase &)  = delete;
        OptionBase & operator=(OptionBase &&)       = default;
        OptionBase(OptionBase &&)                   = default;



        ///////////////////////////////////
        // Virtual functions
        ///////////////////////////////////

        /*! \brief Create a clone of this object
         */
        virtual OptionBasePtr clone(void) const = 0;



        /*! \brief Check if this option has a value
         *
         * \exnothrow
         *
         * \return True if this option has a set value or a default value, false otherwise
         */
        virtual bool has_value(void) const noexcept = 0;



        /*! \brief Check if this option has a default
         *
         * \exnothrow
         *
         * \return True if this option has a default, false otherwise
         */
        virtual bool has_default(void) const noexcept = 0;


        /*! \brief Check if this option is set
         *
         * This is true, even if the option is set explicitly to
         * the default.
         */
        virtual bool is_set(void) const noexcept = 0;


        /*! \brief Check if this option is set to the default
         *
         * Will perform a comparison.
         *
         * \note May be tricky with floating point types.
         */
        virtual bool is_default(void) const = 0;


        /*! \brief Set the option to its default
         *
         * \exnothrow
         */
        virtual void reset_to_default(void) noexcept = 0;



        /*! \brief Get any issues for this option
         *
         * Checks to see if it is set (if required), and calls the validator.
         * Does not throw on validation error, but will throw if the validator
         * cannot be called (python issues, etc).
         *
         * \throw pulsar::PulsarException if there is a problem
         *        with the validation function
         *
         * \return Any problems / issues with this option
         */
        virtual OptionIssues get_issues(void) const = 0;



        /*! \brief Compare with another OptionBase object
         */
        virtual bool compare(const OptionBase & rhs) const = 0;


        /*! \brief Get the type of this option as a string
         *
         * \exnothrow
         */
        virtual const char * type_string(void) const noexcept = 0;


        /*! \brief Get the type of this
         *
         * \exnothrow
         */
        virtual OptionType type(void) const noexcept = 0;


        /*! \brief print out information about this option
         *
         * Will print its key, type, etc. If there is a problem
         * with the option, the Error() output is used. If the
         * option is not the default, the Changed() output is used.
         */
        virtual void print(std::ostream & os) const = 0;



        /////////////////////////////////////////
        // Python-related functions
        /////////////////////////////////////////
        /*! \brief Return a copy of the value as a python object
         *
         * \throw pulsar::PulsarException
         *        If the value does not exist or cannot
         *        be converted to a python object
         */
        virtual pybind11::object get_py(void) const = 0;



        /*! \brief Change the value via a python object
         *
         * \throw pulsar::PulsarException
         *        if there is a problem with the python conversion
         *
         *  \exstrong
         */
        virtual void change_py(const pybind11::object & obj) = 0;




        ///////////////////////////////////
        // Base functions
        ///////////////////////////////////

        /*! \brief Return the key of this option
         *
         * \exnothrow
         */
        const std::string & key(void) const noexcept
        {
            return key_;
        }



        /*! \brief Check if this options is required
         *
         * \exnothrow
         */
        bool is_required(void) const noexcept
        {
            return required_;
        }



        /*! \brief Check to see if this object is set if required
         *
         * \exnothrow
         *
         * \return True if there is a value or a default, or if this
         *         option is not required
         */
        bool is_set_if_required(void) const noexcept
        {
            return has_value() || has_default() || !is_required();
        }




        /*! \brief Get the help string for this option
         *
         * \exnothrow
         */
        const std::string & help(void) const noexcept
        {
            return help_;
        }



        /*! \brief Test if this option has issues
         *
         * \throw pulsar::PulsarException if there is a problem
         *        with the validation function
         */
        bool has_issues(void) const
        {
            return (get_issues().size());
        }


        /*! \brief Add the key and value of this option to a hash
         */
        void hash_value(bphash::Hasher & h) const
        {
            h(key_);
            hash_value_(h);
        }


    protected:
        /*! \brief For serialization only
         *
         * \warning NOT FOR USE OUTSIDE OF SERIALIZATION
         */
        OptionBase() = default;

        /*! \brief Copy constructor
         *
         * Should only be called from clone(), hence it is kept protected
         */
        OptionBase(const OptionBase &) = default;



        //! \name Hashing and Serialization
        ///@{

        DECLARE_SERIALIZATION_FRIENDS
        BPHASH_DECLARE_HASHING_FRIENDS

        virtual void hash(bphash::Hasher & h) const
        {
            h(key_, required_, help_);
            hash_(h);
        }

        /*! \brief Hash only the value of this option */
        virtual void hash_value_(bphash::Hasher & h) const = 0;


        /*! \brief Hash the contents of this option */
        virtual void hash_(bphash::Hasher & h) const = 0;


        template<class Archive>
        void serialize(Archive & ar)
        {
            ar(key_, required_, help_);
        }

        ///@}


    private:
        //! The key of this option
        std::string key_;

        //! Is this option required
        bool required_;

        //! The help string for this option
        std::string help_;


};


} // close namespace detail
} // close namespace pulsar

#endif
