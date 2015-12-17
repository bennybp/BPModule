/*! \file
 *
 * \brief Storage of options for a module (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_OPTIONMAP_HPP_
#define _GUARD_OPTIONMAP_HPP_

#include <map>

#include "bpmodule/datastore/OptionHolder.hpp"
#include "bpmodule/exception/OptionException.hpp"
#include "bpmodule/util/StringUtil.hpp"


namespace bpmodule {
namespace datastore {


//! Holds top-level issues with an OptionMap
typedef std::vector<std::string> WholeOptionMapIssues;


//! Holds all issues related to an OptionMap
struct OptionMapIssues
{
    WholeOptionMapIssues toplevel;                  //!< Issues with the map itself
    std::map<std::string, OptionIssues> optissues;  //!< Issues with any individual options
};




/*! \brief A class for holding options to a module
 *
 * The options are stored as a key -> value map, with the
 * key being a string. The key is case insensitive.
 */
class OptionMap
{
    public:
        OptionMap(void) = default;

        ~OptionMap(void) = default;


        /*! \brief Copy construct
        *
        * Deep copies all the stored options
        */
        OptionMap(const OptionMap & rhs) = default;


        /*! \brief Assignment
        *
        * Deep copies all the stored options
        */
        OptionMap & operator=(const OptionMap & rhs);



        OptionMap(OptionMap && rhs)                  = default;
        OptionMap & operator=(OptionMap && rhs)      = default;



        /*! \brief Return the module key that these options belong to
         */
        const std::string & ModuleKey(void) const noexcept;




        /*! \brief Determine if this object contains a value for a key
         *
         * Checks to see if the map has the key and then if
         * the option for that key has a value or a default
         *
         * \param [in] key The key to the data
         * \return True if the key exists and has a value, false otherwise
         */
        bool Has(const std::string & key) const;



        /*! \brief Determine if this object contains an option with the given key
         *
         * \param [in] key The key to the data
         * \return True if the key exists, false otherwise
         */
        bool HasKey(const std::string & key) const;



        /*! \brief Return the number of elements contained in this object
         *
         * \exnothrow
         *
         * \return Number of elements in this container
         */
        size_t Size(void) const noexcept;



        /*! \brief Check if the option is currently set to the default
         *
         * \throw bpmodule::exception::OptionException if
         *        the key doesn't exist
         */
        bool IsDefault(const std::string & key) const;



        /*! \brief Sets an option to its default
         *
         * \throw bpmodule::exception::OptionException if
         *        the key doesn't exist
         *
         * \exstrong
         */
        void ResetToDefault(const std::string & key);



        /*! \brief Check all options to see if all required options are set
         *
         * \exnothrow
         */
        bool AllReqSet(void) const noexcept;


        /*! \brief Obtain the option keys for all missing required options
         */
        std::vector<std::string> AllMissingReq(void) const;



        /*! \brief Validate this OptionMap
         *
         * Checks to see if all are set, and calls the validator
         * for all options. Also calls the overall map validator.
         *
         * \note This is not meant to be called from python
         *
         * \throw bpmodule::exception::PythonCallException if there is a problem
         *        with calling a validation function
         */
        OptionMapIssues GetIssues(void) const;



        /*! \brief See if there are any issues with this OptionMap
         *
         * \throw bpmodule::exception::PythonCallException if there is a problem
         *        with validation.
         */
        bool HasIssues(void) const;



        /*! \brief Turn on expert mode for all options
         *
         * In expert mode, some failures (mostly invalid options) will print warnings,
         * but an exception will not be thrown
         */
        void SetExpert(bool expert) noexcept;



        /*! \brief Lock the validity of this map
         *
         * If true, this OptionMap to be fixed in a valid state.
         * Additional changes will cause validation to be
         * run and exceptions to possibly be thrown.
         *
         * The validity of this map is checked first if setting to true.
         *
         * \throw bpmodule::exception::PythonCallException if there is a problem
         *        with validation.
         */
        void LockValid(bool lockvalid);



        /*! \brief Validate this map
         *
         * Causes all the problems with this map to be
         * printed out. If there is a problem (and this OptionMap
         * is not in expert mode), an exception is thrown.
         *
         * \throw bpmodule::exception::PythonCallException if there is a problem
         *        with validation.
         */
        void Validate(void) const;



        /*! \brief Dumps the options to the output
        */
        void Print(void) const;



        /*! \brief Obtain the value for an option
         *
         * Will attempt some safe conversions between integer types
         * or between floating point types.
         *
         * \throw bpmodule::exception::OptionException if the
         *        option does not have a value or if the
         *        key does not exist or if the value cannot be
         *        cast to the appropriate type
         */
        template<typename T>
        T Get(const std::string & key) const
        {
            CheckType_<T>();
            try {
                return GetOrThrow_(key).Get<T>();
            }
            catch(const exception::GeneralException & ex)
            {
                // convert to an OptionException and add the key
                throw exception::OptionException(ex, key);
            }
        }



        /*! \brief Change the stored value for a key
         *
         * The data is copied.
         *
         * \throw bpmodule::exception::OptionException if
         *        the value is invalid (and not expert),
         *        if the key does not exist, or the
         *        data cannot be cast to the appropriate type
         *
         * \exstrong
         */
        template<typename T>
        void Change(const std::string & key, const T & value)
        {
            CheckType_<T>();

            try {
                 GetOrThrow_(key).Change(value);
            }
            catch(const exception::GeneralException & ex)
            {
                // convert to an OptionException and add the key
                throw exception::OptionException(ex, key);
            }
        }



        /*! \brief Compare two OptionMap
         * 
         * Checks that all keys exist in both maps and that
         * all values match. It doesn't matter where the value
         * comes from (default or user-supplied), just that
         * the value obtained from Get() match
         */
        bool Compare(const OptionMap & rhs) const; 


        /*! \brief Compare two OptionMap, but only with some keys
         */
        bool CompareSelect(const OptionMap & rhs, const std::vector<std::string> & selection) const;



        void AddOption(const std::string & key, const pybind11::object & def,
                       const pybind11::object & validator,
                       bool required, const std::string & pytype, const std::string & help);





    private:
        //! Function that validates the whole option container
        typedef std::function<WholeOptionMapIssues (const OptionMap &)> WholeOptionValidator;



        //! The module these options belong to
        std::string modulekey_;

        //! If true, don't throw exceptions on validation errors
        bool expert_;

        //! If true, changing the OptionMap causes validation to be run
        bool lockvalid_;

        //! Holds the options
        std::map<std::string, OptionHolder, util::CaseInsensitiveCompare> opmap_;


        //!< Validates the whole options container
        WholeOptionValidator wholevalid_;







        /*! \brief Get an OptionHolder or throw if the key doesn't exist
         *
         * \note Key should already have been transformed to lowercase
         *
         * \throw bpmodule::exception::OptionException
         *        if a key doesn't exist
         */
        const OptionHolder & GetOrThrow_(const std::string & key) const;


         /* \copydoc GetOrThrow_
         */
        OptionHolder & GetOrThrow_(const std::string & key);




        /*! \brief Checks if a given type is valid for an option
         *
         * If the type is not valid, the program will not compile
         */
        template<typename T>
        static void CheckType_(void) noexcept
        {
            //! \todo check types
        }

};



} // close namespace datastore
} // close namespace bpmodule



#endif

