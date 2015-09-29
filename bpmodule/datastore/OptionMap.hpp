/*! \file
 *
 * \brief Storage of options for a module (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_OPTIONMAP_HPP_
#define _GUARD_OPTIONMAP_HPP_

#include <map>

#include "bpmodule/python_helper/BoostPython_fwd.hpp"
#include "bpmodule/datastore/OptionHolder.hpp"


namespace bpmodule {
namespace datastore {


/*! \brief A class for holding options to a module
 *
 * \todo Split out functions into cpp file
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
        OptionMap(const OptionMap & rhs)
        {
            for(const auto & it : rhs.opmap_)
                opmap_.emplace(it.first, detail::OptionBasePtr(it.second->Clone()));
        }


        /*! \brief Assignment
        * 
        * Deep copies (clones) all the stored options
        */
        OptionMap & operator=(const OptionMap & rhs)
        {
            if(this != &rhs)
            {
                opmap_.clear();
                for(const auto & it : rhs.opmap_)
                    opmap_.emplace(it.first, detail::OptionBasePtr(it.second->Clone()));
            }
            return *this;
        }



        OptionMap(OptionMap && rhs)                  = default;
        OptionMap & operator=(OptionMap && rhs)      = default;



        /*! \brief Obtain the value for an option
         * 
         * \throw bpmodule::exception::OptionException if the
         *        option does not have a value or if the 
         *        key does not exist or if the value cannot be 
         *        cast to the appropriate type
         */
        template<typename T>
        T Get(const std::string & key) const
        {
            std::string lkey = LowerString_(key);
            return GetOrThrow_Cast_<T>(lkey)->Get();
        }



        /*! \brief Determine if this object contains a value for a key
         *
         * Checks to see if the map has the key and then if
         * the option for that key has a value or a default
         *
         * \param [in] key The key to the data
         * \return True if the key exists and has a value, false otherwise
         */
        bool Has(const std::string & key) const
        {
            std::string lkey = LowerString_(key);
            if(opmap_.count(lkey) == 0)
                return false;
            return opmap_.at(lkey)->HasValue();
        }


        /*! \brief Determine if this object contains a key
         *
         * Checks to see if the map has the key
         *
         * \param [in] key The key to the data
         * \return True if the key exists, false otherwise
         */
        bool HasKey(const std::string & key) const
        {
            std::string lkey = LowerString_(key);
            return opmap_.count(lkey);
        }



        /*! \brief Return the number of elements contained
         *
         * \exnothrow
         *
         * \return Number of elements in this container
         */
        size_t Size(void) const noexcept
        {
            return opmap_.size();
        }



        /*! \brief Determine if this object contains an option of a specific type for a key
         *
         * \tparam T Type to compare to
         *
         * \param [in] key The key to the data
         * \return True if the key exists and is of type T, false otherwise
         */
        template<typename T>
        bool HasType(const std::string & key) const
        {
            std::string lkey = LowerString_(key);
            if(!HasKey(lkey))
                return false;

            return GetOrThrow_(lkey)->IsType<T>();
        }



        /*! \brief Get a string representing the type for a given key
         *
         * \throw bpmodule::exception::OptionException
         *        if the key doesn't exist 
         *
         * \param [in] key The key to the data
         * \return A string representing the type for a key
         */
        std::string GetType(const std::string & key) const
        {
            std::string lkey = LowerString_(key);
            return GetOrThrow_(lkey)->Type();
        }



        /*! \brief Check if the option is currently set to the default
         */ 
        bool IsDefault(const std::string & key) const
        {
            std::string lkey = LowerString_(key);
            return GetOrThrow_(lkey)->IsDefault();
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
            std::string lkey = LowerString_(key);
            GetOrThrow_Cast_<T>(lkey)->Change(value);
        }




        /*! \brief Sets an option to its default
         *
         * \throw bpmodule::exception::OptionException if
         *        the option does not have a default or the
         *        key doesn't exist
         *
         * \exstrong 
         */ 
        void ResetToDefault(const std::string & key)
        {
            std::string lkey = LowerString_(key);
            GetOrThrow_(lkey)->ResetToDefault();
        }



        /*! \brief Check all options to see if they are valid
         * 
         * \exnothrow
         */
        bool IsValid(void) const noexcept
        {
            for(const auto & it : opmap_)
                if(!it.second->IsValid())
                    return false;
            return true;
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
         */ 
        OptionMap(const boost::python::dict & opt);


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
        boost::python::object GetPy(const std::string & key) const
        {
            std::string lkey = LowerString_(key);
            return GetOrThrow_(lkey)->GetPy();
        }



    private:
        std::map<std::string, detail::OptionBasePtr> opmap_;


        /*! \brief Get an OptionBasePtr or throw if the key doesn't exist
         * 
         * \note Key should already have been transformed to lowercase
         *
         * \throw bpmodule::exception::OptionException
         *        if a key doesn't exist
         */
        detail::OptionBasePtr & GetOrThrow_(const std::string & key)
        {
            if(opmap_.count(key))
                return opmap_.at(key);
            else
                throw exception::GeneralException("Key not found",
                                                  "optionkey", key); 
        }



        //! \copydoc GetOrThrow_
        const detail::OptionBasePtr & GetOrThrow_(const std::string & key) const
        {
            if(opmap_.count(key))
                return opmap_.at(key);
            else
                throw exception::GeneralException("Key not found",
                                                  "optionkey", key); 
        }




        /*! \brief Get an OptionBasePtr and cast to an appropriate OptionHolder
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
            const detail::OptionBasePtr & ptr = GetOrThrow_(key);
            const detail::OptionHolder<T> * oh = dynamic_cast<const detail::OptionHolder<T> *>(ptr.get());
            if(oh == nullptr)
                throw exception::OptionException("Bad cast",
                                                 "optionkey", key,
                                                 "fromtype", ptr->Type(),
                                                 "totype", typeid(T).name()); 

            return oh;
        }


        /*! \brief Converts a string to lower case
         */ 
        static std::string LowerString_(const std::string & str)
        {
            //! \todo assume ASCII
            std::string ret(str);
            std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
            return ret;
        }
};


} // close namespace datastore
} // close namespace bpmodule



#endif

