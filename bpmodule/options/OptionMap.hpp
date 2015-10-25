/*! \file
 *
 * \brief Storage of options for a module (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_OPTIONMAP_HPP_
#define _GUARD_OPTIONMAP_HPP_

#include <map>

#include "bpmodule/options/OptionTypes.hpp"
#include "bpmodule/options/OptionHolder.hpp"
#include "bpmodule/exception/OptionException.hpp"
#include "bpmodule/mangle/Mangle.hpp"
#include "bpmodule/output/StrHelp.hpp"


namespace bpmodule {
namespace options {



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
         */
        bool IsDefault(const std::string & key) const;



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


        /*! \brief Obtain the option keys for all missing options
         */
        std::vector<std::string> AllMissingReq(void) const;



        /*! \brief Turn on expert mode for all options
         * 
         * In expert mode, failures (validation, etc) will print warnings,
         * but the program will otherwise continue
         */
        void SetExpert(bool expert) noexcept;


        /*! \brief Check the validity of all the options
         *
         */
        //bool IsValid(void) const; 


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



        /*! \brief Dumps the options to the output
        */
        void Print(void) const;



        /////////////////////////////
        // Python-related functions
        /////////////////////////////
        /*! \brief Construct options from a python dictionary
         *
         * \throw bpmodule::exception::OptionException if there is
         *        a problem with the option (validation, conversion, duplicate key, etc)
         *
         *  \param [in] opt A dictionary with the options
         *  \param [in] wholevalidfunc Pointer to a function to validate the whole options object
         */
        OptionMap(const boost::python::dict & opt, const boost::python::object & wholevalidfunc);



        /*! \brief Change options via python dictionary
         *
         * Dictionary is simple string key -> value mapping.
         *
         * \throw bpmodule::exception::OptionException if there is
         *        a problem with the option (nonexistant key, validation, conversion, duplicate key, etc)
         *
         * \exstrong
         */
        void ChangePyDict(const boost::python::dict & opt);



        /*! \brief Change an option by passing a boost::python object
         *
         * \throw bpmodule::exception::OptionException if there is
         *        a problem with the option (nonexistant key, validation, conversion, etc)
         *
         * \exstrong
         */
        void ChangePy(const std::string & key, const boost::python::object & obj);



        /*! \brief Return the option's value as a python object
         *
         * \throw bpmodule::exception::OptionException if there is
         *        a problem with the option (nonexistant key, validation, conversion, etc)
         */
        boost::python::object GetPy(const std::string & key) const;





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

        //! If true, don't throw exceptions on validation errors
        bool expert_;

        //! Function that validates the whole option container
        typedef std::function<bool (const OptionMap &)> WholeOptionValidator;


        //! \brief Comparison of a case-insensitive string
        struct CaseInsensitiveCompare_
        {
            bool operator()(std::string lhs, std::string rhs) const
            {
                output::ToLower(lhs);
                output::ToLower(rhs);
                std::less<std::string> comp;
                return comp(lhs, rhs);
            }
        }; 


        //! Holds the options
        std::map<std::string, detail::OptionBasePtr, CaseInsensitiveCompare_> opmap_;
 

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
                                                 "totype", mangle::DemangleCppType<T>());

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
                                                 "totype", mangle::DemangleCppType<T>());

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



} // close namespace options
} // close namespace bpmodule



#endif

