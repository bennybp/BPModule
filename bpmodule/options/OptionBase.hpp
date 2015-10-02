/*! \file
 *
 * \brief Storage of generic data (base class) 
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_OPTIONBASE_HPP_
#define _GUARD_OPTIONBASE_HPP_

#include <memory>
#include <cstring>
#include <string>

#include "bpmodule/python_helper/BoostPython_fwd.hpp"

namespace bpmodule {
namespace options {
namespace detail {


class OptionBase
{
    public:
        /*! \brief Constructor
         *
         * Expert is set to false.
         *
         * \param [in] key The key of this option
         * \param [in] required True if this is a required option
         * \param [in] pytype The python type of this option
         * \param [in] help A help string for this option
         */
        OptionBase(const std::string & key,
                   bool required,
                   python_helper::PythonType pytype,
                   const std::string & help);

        virtual ~OptionBase() noexcept    = default;

        OptionBase & operator=(const OptionBase & rhs)  = delete;
        OptionBase & operator=(OptionBase && rhs)       = delete;
        OptionBase(const OptionBase & rhs)              = default;
        OptionBase(const OptionBase && rhs)             = delete;



        ///////////////////////////////////
        // Virtual functions
        ///////////////////////////////////
        
        /*! \brief Create a clone of this object
         */
        virtual OptionBase * Clone(void) const = 0;



        /*! \brief Returns a string representing the type of the option stored
         *
         * \exnothrow
         *
         * \return A string representing the type (obtained via typeid().name())
         */
        virtual const char * Type(void) const noexcept = 0;



        /*! \brief Check if this option has a value
         * 
         * \exnothrow
         *
         * \return True if this option has a value or a default
         */
        virtual bool HasValue(void) const noexcept = 0;


        /*! \brief Check if this option has a default
         * 
         * \exnothrow
         *
         * \return True if this option has a default
         */
        virtual bool HasDefault(void) const noexcept = 0;



        /*! \brief Check if this option is set to the default
         */ 
        virtual bool IsDefault(void) const = 0;



        /*! \brief Set the option to its default
         */ 
        virtual void ResetToDefault(void) noexcept = 0;



        /*! \brief Print the option information
         */ 
        virtual void Print(void) const = 0;


        /////////////////////////////////////////
        // Python-related functions
        /////////////////////////////////////////
        /*! \brief Return a copy of the value as a boost::python object
         *
         * \throw bpmodule::exception::OptionException 
         *        If the value does not exist or cannot
         *        be converted to a python object
         */ 
        virtual boost::python::object GetPy(void) const = 0;



        /*! \brief Change the value with a boost python object
         *
         * \throw bpmodule::exception::OptionException                 
         *        if there is a problem with the python conversion or
         *        if the new value is invalid (and expert mode is off).
         *
         *  \exstrong
         */  
        virtual void ChangePy(const boost::python::object & obj) = 0;




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



        /*! \brief See if expert mode is enabled for this object
         *
         * \exnothrow 
         */
        bool IsExpert(void) const noexcept;



        /*! \brief Set the expert variable
         */
        void SetExpert(bool expert) noexcept;


        /*! \brief Check to see if this object is valid
         *
         * \exnothrow 
         * 
         * \return True if there is a value or a default, or if this
         *         option is not required
         */
        bool IsValid(void) const noexcept;



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
           return (strcmp(typeid(U).name(), Type()) == 0);
        }


        /*! \brief Get the python type of this option
         */
        python_helper::PythonType PyType(void) const noexcept;
        
 
        /*! \brief Get the help string for this option
         */
        const std::string & Help(void) const noexcept;


    private:
        //! The key of this option
        const std::string key_;

        //! Is this option required
        const bool required_;

        //! If set to true, validation errors print warnings rather than throw exceptions.
        bool expert_;

        //! The python type of this option
        const python_helper::PythonType pytype_;

        //! The help string for this option
        const std::string help_;

    

};


//! A pointer to an option
typedef std::shared_ptr<OptionBase> OptionBasePtr;





} //closing namespace detail
} //closing namespace options
} //closing namespace bpmodule

#endif
