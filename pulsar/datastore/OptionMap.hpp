/*! \file
 *
 * \brief Storage of options for a module (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_DATASTORE__OPTIONMAP_HPP_
#define PULSAR_GUARD_DATASTORE__OPTIONMAP_HPP_

#include <map>

#include "pulsar/datastore/OptionTypes.hpp"
#include "pulsar/datastore/OptionHolder.hpp"
#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/util/StringUtil.hpp"


namespace pulsar{
namespace datastore {


//! Significant Options to compare
typedef std::set<std::string> KeySet;


//! Holds top-level issues with an OptionMap
typedef std::vector<std::string> WholeOptionMapIssues;


//! Holds all issues related to an OptionMap
struct OptionMapIssues
{
    WholeOptionMapIssues toplevel;                  //!< Issues with the map itself
    std::map<std::string, OptionIssues> optissues;  //!< Issues with any individual options

    /*! \brief Check to see if there are issues
     *
     * Returns true if there are no issues stored in this structure
     */
    bool OK(void) const;


    /*! \brief Print issues (if there are any) */
    void Print(std::ostream & os) const;

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

        OptionMap(const std::string & modulekey);

        ~OptionMap(void) = default;


        /*! \brief Copy construct
        *
        * Deep copies all the stored options
        */
        OptionMap(const OptionMap & rhs);


        /*! \brief Assignment
        *
        * Deep copies all the stored options
        */
        OptionMap & operator=(const OptionMap & rhs);



        OptionMap(OptionMap &&)                  = default;
        OptionMap & operator=(OptionMap &&)      = default;



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
         * \throw pulsar::exception::OptionException if
         *        the key doesn't exist
         */
        bool IsDefault(const std::string & key) const;




        /*! \brief Check to see if an option is set
         * 
         * Ie, is the option set by the user. This is true even
         * if it is explicitly set to the default value.
         */
        bool IsSet(const std::string & key) const;



        /*! \brief Sets an option to its default
         *
         * \throw pulsar::exception::OptionException if
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
        KeySet AllMissingReq(void) const;



        /*! \brief Validate this OptionMap
         *
         * Checks to see if all are set, and calls the validator
         * for all options. Also calls the overall map validator.
         *
         * \note This is not meant to be called from python
         *
         * \throw pulsar::exception::PythonCallException if there is a problem
         *        with calling a validation function
         */
        OptionMapIssues GetIssues(void) const;



        /*! \brief See if there are any issues with this OptionMap
         *
         * \throw pulsar::exception::PythonCallException if there is a problem
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
         * \throw pulsar::exception::PythonCallException if there is a problem
         *        with validation.
         */
        void LockValid(bool lockvalid);


        /*! \brief Dumps the options to the output
        */
        void Print(std::ostream & os) const;



        /*! \brief Obtain the value for an option
         *
         * Will attempt some safe conversions between integer types
         * or between floating point types.
         *
         * \throw pulsar::exception::OptionException if the
         *        option does not have a value or if the
         *        key does not exist or if the value cannot be
         *        cast to the appropriate type
         */
        template<typename T>
        T Get(const std::string & key) const
        {
            static constexpr OptionType opt_type = OptionTypeMap<T>::opt_type;
            typedef typename OptionTypeInfo<opt_type>::stored_type stored_type;

            CheckType_<T>();

            stored_type val = GetOrThrow_Cast_<opt_type>(key)->Get();

            try {
                return OptionCast<T,stored_type>::Cast(val);
            }
            catch(const std::exception & ex)
            {
                throw exception::OptionException(ex, "optionkey", key, "modulekey", modulekey_);
            }
        }



        /*! \brief Change the stored value for a key
         *
         * The data is copied.
         *
         * \throw pulsar::exception::OptionException if
         *        the value is invalid (and not expert),
         *        if the key does not exist, or the
         *        data cannot be cast to the appropriate type
         *
         * \exstrong
         */
        template<typename T>
        void Change(const std::string & key, const T & value)
        {
            static constexpr OptionType opt_type = OptionTypeMap<T>::opt_type;
            typedef typename OptionTypeInfo<opt_type>::stored_type stored_type;

            CheckType_<T>();

            stored_type convval;

            try {
                 convval = OptionCast<stored_type, T>::Cast(value);
            }
            catch(const std::exception & ex)
            {
                // convert to an OptionException and add the key
                throw exception::OptionException(ex, "optionkey", key, "modulekey", modulekey_);
            }

            GetOrThrow_Cast_<opt_type>(key)->Change(convval);

            if(lockvalid_)
                Validate_();
        }



        /*! \brief Compare two OptionMap
         * 
         * Checks that all keys exist in both maps and that
         * all values match. It doesn't matter where the value
         * comes from (default or user-supplied), just that
         * the value obtained from Get() match
         */
        bool Compare(const OptionMap & rhs) const; 


        /*! \copydocs Compare(const OptionMap & rhs) const
         * 
         * \todo does this make sense?
         */
        bool operator==(const OptionMap & rhs) const;


        /*! \brief Compare two OptionMap, but only with some keys
         */
        bool CompareSelect(const OptionMap & rhs, const KeySet & selection) const;



        /*! \brief Add an option to this map
         *
         * \todo exceptions 
         */
        void AddOption(std::string key, OptionType opttype, bool required,
                       const pybind11::object & validator, std::string help,
                       const pybind11::object & def);




        /////////////////////////////
        // Python
        /////////////////////////////
        /*! \brief Change an option by passing a boost::python object
         *
         * \throw pulsar::exception::OptionException if there is
         *        a problem with the option (nonexistant key, validation, conversion, etc)
         *
         * \throw pulsar::exception::PythonCallException if there is a problem
         *        with the validation itself.
         *
         * \exstrong
         */
        void ChangePy(const std::string & key, const pybind11::object & obj);



        /*! \brief Return the option's value as a python object
         *
         * \throw pulsar::exception::OptionException if there is
         *        a problem with the option (nonexistant key, validation, conversion, etc)
         */
        pybind11::object GetPy(const std::string & key) const;



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
        std::map<std::string, std::unique_ptr<OptionBase>, util::CaseInsensitiveLess> opmap_;

        //!< Validates the whole options container
        WholeOptionValidator wholevalid_;





        /*! \brief Get an OptionBase or throw if the key doesn't exist
         *
         * \note Key should already have been transformed to lowercase
         *
         * \throw pulsar::exception::OptionException
         *        if a key doesn't exist
         */
        const OptionBase * GetOrThrow_(const std::string & key) const;


         /* \copydoc GetOrThrow_
         */
        OptionBase * GetOrThrow_(const std::string & key);


        /*! \brief Get a pointer to OptionBase and cast it to an appropriate OptionBase
         *
         * \note Key should already have been transformed to lowercase
         *
         * \throw pulsar::exception::OptionException
         *        if a key doesn't exist or cannot
         *        be cast to the desired type
         */
        template<OptionType OPTTYPE>
        const OptionHolder<OPTTYPE> * GetOrThrow_Cast_(const std::string & key) const
        {
            const OptionBase * ptr = GetOrThrow_(key);
            const OptionHolder<OPTTYPE> * oh = dynamic_cast<const OptionHolder<OPTTYPE> *>(ptr);
            if(oh == nullptr)
                throw exception::OptionException("Bad option cast", "optionkey", key,
                                                 "modulekey", modulekey_,
                                                 "fromtype", ptr->TypeString(),
                                                 "totype", OptionTypeToString(OPTTYPE)); 

            return oh;
        }


        /*! \copydoc GetOrThrow_Cast_
         */
        template<OptionType OPTTYPE>
        OptionHolder<OPTTYPE> * GetOrThrow_Cast_(const std::string & key)
        {
            OptionBase * ptr = GetOrThrow_(key);
            OptionHolder<OPTTYPE> * oh = dynamic_cast<OptionHolder<OPTTYPE> *>(ptr);
            if(oh == nullptr)
                throw exception::OptionException("Bad option cast", "optionkey", key,
                                                 "modulekey", modulekey_,
                                                 "fromtype", ptr->TypeString(),
                                                 "totype", OptionTypeToString(OPTTYPE)); 

            return oh;
        }



        /*! \brief Checks if a given type is valid for an option
         *
         * If the type is not valid, the program will not compile
         */
        template<typename T>
        static void CheckType_(void) noexcept
        {
            /*! \todo - check types for better compilation error messages */
            //static_assert( OptionTypeMap<T>::valid,
            //               "Invalid type for an option given to OptionMap");
        }


        /*! \brief Validate and throw an exception if necessary
         * 
         * Validation errors will throw an exception if not in expert mode
         */
        void Validate_(void) const;



        //! \name Serialization
        ///@{

        DECLARE_SERIALIZATION_FRIENDS


        template<class Archive>
        void save(Archive & ar) const
        {
            // We don't do lockvalid - if we are unserializing, we are
            // going to lock it. We also don't do validators
            ar(modulekey_, expert_);

            // We have to do the options a special way
            // static cast for size - just to be absolutely sure
            ar(static_cast<size_t>(opmap_.size()));
            for(const auto & it : opmap_)
                ar(it.first, it.second->Type(), it.second->ToByteArray());
        }

        template<class Archive>
        void load(Archive & ar)
        {
            size_t size;
            ar(modulekey_, expert_, size);

            // We have to do the options a special way
            for(size_t i = 0; i < size; i++)
            {
                std::string key;
                OptionType opttype;
                ByteArray ba;
                ar(key, opttype, ba);
                std::unique_ptr<OptionBase> opt = OptionHolderFromByteArray(opttype, ba);
                opmap_.emplace(key, std::move(opt));
            }

            lockvalid_ = true;
        }

        ///@}
};



} // close namespace datastore
} // close namespace pulsar



#endif

