/*! \file
 *
 * \brief Storage of generic data (base class) (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_OPTIONBASE_HPP_
#define _GUARD_OPTIONBASE_HPP_

#include <memory>
#include <string>
#include <vector>
#include <typeinfo>

#include "bpmodule/python/Pybind11.hpp"
#include "bpmodule/python/Types.hpp"

namespace bpmodule {
namespace datastore {


// forward declare class
class OptionBase;



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
         * \param [in] pytype The python type of this option
         * \param [in] required True if this is a required option
         * \param [in] help A help string for this option
         */
        OptionBase(const std::string & key,
                   python::PythonType pytype,
                   bool required,
                   const std::string & help);

        virtual ~OptionBase() noexcept                  = default;

        // Don't allow copying / assigning
        OptionBase & operator=(const OptionBase & rhs)  = delete;
        OptionBase & operator=(OptionBase && rhs)       = delete;
        OptionBase(const OptionBase && rhs)             = delete;



        ///////////////////////////////////
        // Virtual functions
        ///////////////////////////////////

        /*! \brief Create a clone of this object
         */
        virtual OptionBasePtr Clone(void) const = 0;



        /*! \brief Returns a string representing the type of the stored option
         *
         * \exnothrow
         *
         * \return A string representing the type (obtained via typeid().name())
         */
        virtual const char * Type(void) const noexcept = 0;



        /*! \brief Returns the std::type_info for the stored option
         *
         * \exnothrow
         *
         * \return A std::type_info structure representing the stored type
         */
        virtual const std::type_info & TypeInfo(void) const noexcept = 0;



        /*! \brief Returns a string representing the demangled type of the stored option
         *
         * \return A string representing the type (obtained via typeid().name(), but demangled)
         */
        virtual std::string DemangledType(void) const = 0;



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
         * \throw bpmodule::exception::PythonCallException if there is a problem
         *        with the validation function
         *
         * \return Any problems / issues with this option
         */
        virtual OptionIssues GetIssues(void) const = 0;



        /*! \brief Compare with another OptionBase object
         */
        virtual bool Compare(const OptionBase & rhs) const = 0; 



        /*! \brief Print out information about this option
         */
        virtual void Print(void) const = 0;




        /////////////////////////////////////////
        // Python-related functions
        /////////////////////////////////////////
        /*! \brief Return a copy of the value as a python object
         *
         * \throw bpmodule::exception::OptionException
         *        If the value does not exist or cannot
         *        be converted to a python object
         */
        virtual pybind11::object GetPy(void) const = 0;



        /*! \brief Change the value via a python object
         *
         * \throw bpmodule::exception::OptionException
         *        if there is a problem with the python conversion
         *
         *  \exstrong
         */
        virtual void ChangePy(pybind11::object obj) = 0;




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
           return typeid(U) == TypeInfo();
        }



        /*! \brief Get the python type of this option
         *
         * \exnothrow
         */
        python::PythonType PyType(void) const noexcept;



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


    protected:
        /*! \brief Copy constructor
         *
         * Should only be called from Clone(), hence it is kept protected
         */
        OptionBase(const OptionBase & rhs) = default;



    private:
        //! The key of this option
        const std::string key_;

        //! The python type of this option
        const python::PythonType pytype_;

        //! Is this option required
        const bool required_;

        //! The help string for this option
        const std::string help_;



};






} //closing namespace datastore
} //closing namespace bpmodule

#endif
