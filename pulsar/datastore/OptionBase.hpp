/*! \file
 *
 * \brief Storage of options data (base class) (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_DATASTORE__OPTIONBASE_HPP_
#define PULSAR_GUARD_DATASTORE__OPTIONBASE_HPP_

#include <memory>
#include <string>
#include <vector>
#include <ostream>

#include "pulsar/python/Pybind11_fwd.hpp"
#include "pulsar/util/Serialization.hpp"
#include "pulsar/util/bphash/Hasher_fwd.hpp"

namespace pulsar{
namespace datastore {


// forward declare class
class OptionBase;
enum class OptionType;



//! A collection of problems with an option
typedef std::vector<std::string> OptionIssues;


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
        OptionBase(const std::string & key, bool required, const std::string & help);

        /// \copydocs OptionBase(const std::string &, bool, const std::string &)
        OptionBase(std::string && key, bool required, std::string && help);


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
        virtual OptionBasePtr Clone(void) const = 0;



        /*! \brief Check if this option has a value
         *
         * \exnothrow
         *
         * \return True if this option has a set value or a default value, false otherwise
         */
        virtual bool HasValue(void) const noexcept = 0;



        /*! \brief Check if this option has a default
         *
         * \exnothrow
         *
         * \return True if this option has a default, false otherwise
         */
        virtual bool HasDefault(void) const noexcept = 0;


        /*! \brief Check if this option is set
         *
         * This is true, even if the option is set explicitly to
         * the default.
         */
        virtual bool IsSet(void) const noexcept = 0;


        /*! \brief Check if this option is set to the default
         *
         * Will perform a comparison.
         *
         * \note May be tricky with floating point types.
         */
        virtual bool IsDefault(void) const = 0;


        /*! \brief Set the option to its default
         *
         * \exnothrow
         */
        virtual void ResetToDefault(void) noexcept = 0;



        /*! \brief Get any issues for this option
         *
         * Checks to see if it is set (if required), and calls the validator.
         * Does not throw on validation error, but will throw if the validator
         * cannot be called (python issues, etc).
         *
         * \throw pulsar::exception::PythonCallException if there is a problem
         *        with the validation function
         *
         * \return Any problems / issues with this option
         */
        virtual OptionIssues GetIssues(void) const = 0;



        /*! \brief Compare with another OptionBase object
         */
        virtual bool Compare(const OptionBase & rhs) const = 0; 


        /*! \brief Get the type of this option as a string
         *
         * \exnothrow
         */
        virtual const char * TypeString(void) const noexcept = 0;


        /*! \brief Get the type of this
         *
         * \exnothrow
         */
        virtual OptionType Type(void) const noexcept = 0;


        /*! \brief Print out information about this option
         */
        virtual void Print(std::ostream & os) const = 0;


        /*! \brief Serialize the option to a byte array */
        virtual ByteArray ToByteArray(void) const = 0;



        /////////////////////////////////////////
        // Python-related functions
        /////////////////////////////////////////
        /*! \brief Return a copy of the value as a python object
         *
         * \throw pulsar::exception::OptionException
         *        If the value does not exist or cannot
         *        be converted to a python object
         */
        virtual pybind11::object GetPy(void) const = 0;



        /*! \brief Change the value via a python object
         *
         * \throw pulsar::exception::OptionException
         *        if there is a problem with the python conversion
         *
         *  \exstrong
         */
        virtual void ChangePy(const pybind11::object & obj) = 0;




        ///////////////////////////////////
        // Base functions
        ///////////////////////////////////

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
         * \exnothrow
         *
         * \return True if there is a value or a default, or if this
         *         option is not required
         */
        bool IsSetIfRequired(void) const noexcept;




        /*! \brief Get the help string for this option
         *
         * \exnothrow
         */
        const std::string & Help(void) const noexcept;



        /*! \brief Test if this option has issues
         *
         * \throw pulsar::exception::PythonCallException if there is a problem
         *        with the validation function
         */
        bool HasIssues(void) const;


        util::Hash MyHash(void) const;


    protected:
        /*! \brief For serialization only
         * 
         * \warning NOT FOR USE OUTSIDE OF SERIALIZATION
         */
        OptionBase() = default;

        /*! \brief Copy constructor
         *
         * Should only be called from Clone(), hence it is kept protected
         */
        OptionBase(const OptionBase &) = default;


        virtual void hash_value(util::Hasher & h) const = 0;



    private:
        //! The key of this option
        std::string key_;

        //! Is this option required
        bool required_;

        //! The help string for this option
        std::string help_;

        //! \name Hashing and Serialization
        ///@{

        DECLARE_HASHING_FRIENDS

        virtual void hash(util::Hasher & h) const;

        ///@}

};






} //closing namespace datastore
} //closing namespace pulsar

#endif
