/*! \file
 *
 * \brief Storage of options for a module (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_OPTIONMAP_HPP_
#define _GUARD_OPTIONMAP_HPP_

#include <map>

#include "bpmodule/datastore/OptionTypes.hpp"
#include "bpmodule/datastore/OptionHolder.hpp"
#include "bpmodule/exception/OptionException.hpp"
#include "bpmodule/util/StringUtil.hpp"
#include "bpmodule/python_helper/Pybind11.hpp"


namespace bpmodule {
namespace datastore {


//! Holds top-level issues with an OptionMap
typedef std::vector<std::string> WholeOptionMapIssues;


//! Holds all issues related to an OptionMap
struct OptionMapIssues
{
    WholeOptionMapIssues toplevel;                          //!< Issues with the map itself
    std::map<std::string, detail::OptionIssues> optissues;  //!< Issues with any individual options
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
        * Deep copies (clones) all the stored options
        */
        OptionMap(const OptionMap & rhs);


        /*! \brief Assignment
        *
        * Deep copies (clones) all the stored options
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



        /*! \brief Check if the map has a key with a given type
         *
         *  Does not check for validity or if the stored value
         *  can be successfully converted (ie, overflow)
         */
        template<typename T>
        bool HasType(const std::string & key) const
        {
            typedef typename detail::OptionConvert<T>::stored_type stored_type;

            CheckType_<T>();

            if(!HasKey(key))
                return false;

            return GetOrThrow_(key)->IsType<stored_type>();
        }




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
            typedef typename detail::OptionConvert<T>::stored_type stored_type;

            CheckType_<T>();

            stored_type val = GetOrThrow_Cast_<stored_type>(key)->Get();

            try {
                return detail::OptionConvert<T>::ConvertFromStored(val);
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
            typedef typename detail::OptionConvert<T>::stored_type stored_type;

            CheckType_<T>();
            stored_type convval;

            try {
                 convval = detail::OptionConvert<T>::ConvertToStored(value);
            }
            catch(const exception::GeneralException & ex)
            {
                // convert to an OptionException and add the key
                throw exception::OptionException(ex, key);
            }

            GetOrThrow_Cast_<stored_type>(key)->Change(convval);
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


        /////////////////////////////
        // Python-related functions
        /////////////////////////////
        /*! \brief Construct options from a python dictionary
         *
         * \throw bpmodule::exception::OptionException if there is
         *        a problem with the option (validation, conversion, etc)
         *
         * \throw bpmodule::exception::PythonCallException if there is a problem
         *        with the validation itself.
         *
         *  \param [in] opt A dictionary with the options
         *  \param [in] modulekey The module key of the the module that has these options
         *  \param [in] wholevalidfunc Pointer to a function to validate the whole options object
         *  \param [in] modulekey The module key that these options belong to
         */
        OptionMap(const std::string & modulekey, pybind11::dict opt, pybind11::object wholevalidfunc);



        /*! \brief Change an option by passing a python object
         *
         * \throw bpmodule::exception::OptionException if there is
         *        a problem with the option (nonexistant key, validation, conversion, etc)
         *
         * \throw bpmodule::exception::PythonCallException if there is a problem
         *        with the validation itself.
         *
         * \exstrong
         */
        void ChangePy(const std::string & key, pybind11::object obj);



        /*! \brief Change options via python dictionary
         *
         * Dictionary is simple string key -> value mapping.
         *
         * \throw bpmodule::exception::OptionException if there is
         *        a problem with the option (nonexistant key, validation, conversion, etc)
         *
         * \throw bpmodule::exception::PythonCallException if there is a problem
         *        with the validation itself.
         *
         * \exstrong
         */
        void ChangePyDict(pybind11::dict opt);



        /*! \brief Return the option's value as a python object
         *
         * \throw bpmodule::exception::OptionException if there is
         *        a problem with the option (nonexistant key, validation, conversion, etc)
         */
        pybind11::object GetPy(const std::string & key) const;



        /*! \brief Compare two OptionMap, but only with some keys
         */
        bool CompareSelectPy(const OptionMap & rhs, pybind11::list selection) const;


        //////////////////////////////////////////
        // these aren't static constexpr
        // since the are to be called from python
        //////////////////////////////////////////
        /*! \brief Return the maximum value for an integer that can be stored in this OptionMap
         */
        detail::OptionInt MaxInt(void) const
        {
            return std::numeric_limits<detail::OptionInt>::max();
        }


        /*! \brief Return the minimum value for an integer that can be stored in this OptionMap
         */
        detail::OptionInt MinInt(void) const
        {
            return std::numeric_limits<detail::OptionInt>::lowest();
        }


        /*! \brief Return the maximum value for an floating point that can be stored in this OptionMap
         */
        detail::OptionFloat MaxFloat(void) const
        {
            return std::numeric_limits<detail::OptionFloat>::max();
        }


        /*! \brief Return the minimum value for a floating point that can be stored in this OptionMap
         */
        detail::OptionFloat MinFloat(void) const
        {
            return std::numeric_limits<detail::OptionFloat>::lowest();
        }


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
        std::map<std::string, detail::OptionBasePtr, util::CaseInsensitiveCompare> opmap_;


        //!< Validates the whole options container
        WholeOptionValidator wholevalid_;







        /*! \brief Get an pointer to OptionBase or throw if the key doesn't exist
         *
         * \note Key should already have been transformed to lowercase
         *
         * \throw bpmodule::exception::OptionException
         *        if a key doesn't exist
         */
        detail::OptionBase * GetOrThrow_(const std::string & key);



        //! \copydoc GetOrThrow_
        const detail::OptionBase * GetOrThrow_(const std::string & key) const;




        /*! \brief Get a pointer to OptionBase and cast it to an appropriate OptionHolder
         *
         * \note Key should already have been transformed to lowercase
         *
         * \throw bpmodule::exception::OptionException
         *        if a key doesn't exist or cannot
         *        be cast to the desired type
         */
        template<typename T>
        const detail::OptionHolder<T> * GetOrThrow_Cast_(const std::string & key) const
        {
            CheckType_<T>();
            const detail::OptionBase * ptr = GetOrThrow_(key);
            const detail::OptionHolder<T> * oh = dynamic_cast<const detail::OptionHolder<T> *>(ptr);
            if(oh == nullptr)
                throw exception::OptionException("Bad cast", key,
                                                 "fromtype", ptr->DemangledType(),
                                                 "totype", util::DemangleCppType<T>());

            return oh;
        }


        /*! \copydoc GetOrThrow_Cast_
         */
        template<typename T>
        detail::OptionHolder<T> * GetOrThrow_Cast_(const std::string & key)
        {
            CheckType_<T>();
            detail::OptionBase * ptr = GetOrThrow_(key);
            detail::OptionHolder<T> * oh = dynamic_cast<detail::OptionHolder<T> *>(ptr);
            if(oh == nullptr)
                throw exception::OptionException("Bad cast", key,
                                                 "fromtype", ptr->DemangledType(),
                                                 "totype", util::DemangleCppType<T>());

            return oh;
        }



        /*! \brief Checks if a given type is valid for an option
         *
         * If the type is not valid, the program will not compile
         */
        template<typename T>
        static void CheckType_(void) noexcept
        {
            static_assert( detail::IsValidType<T>::value,
                           "Invalid type for an option given to OptionMap");
        }

};



} // close namespace datastore
} // close namespace bpmodule



#endif

