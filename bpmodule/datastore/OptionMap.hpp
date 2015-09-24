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
#include "bpmodule/exception/MapException.hpp"


namespace bpmodule {
namespace datastore {


/*! \brief A class for holding options to a module
 *
 * \todo Copy and swap? Swap function?
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
         *        option does not have a value
         *
         * \throw bpmodule::exception::MapException if the
         *        key does not exist
         */
        template<typename T>
        T Get(const std::string & key) const
        {
            return GetOrThrow_Cast_<T>(key)->GetValue();
        }



        /*! \brief Determine if this object contains a value for a key
         *
         * Checks to see if the map has the key and then if
         * the option for that key has a value or a default
         *
         * \param [in] key The key to the data
         * \return True if the key exists, false otherwise
         */
        bool Has(const std::string & key) const
        {
            if(opmap_.count(key) == 0)
                return false;
            return opmap_.at(key)->HasValue();
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
            if(!Has(key))
                return false;

            return GetOrThrow_(key)->IsType<T>();
        }



        /*! \brief Get a string representing the type for a given key
         *
         * \throw bpmodule::exception::MapException
         *        if the key doesn't exist 
         *
         * \param [in] key The key to the data
         * \return A string representing the type for a key
         */
        std::string GetType(const std::string & key) const
        {
            return GetOrThrow_(key)->Type();
        }



        /*! \brief Check if the option is currently set to the default
         */ 
        bool IsDefault(const std::string & key) const
        {
            return GetOrThrow_(key)->IsDefault();
        }


        /*! \brief Change the stored value for a key
         *
         * The data is copied.
         *
         * \throw bpmodule::exception::MapException
         *        if the key doesn't exist 
         *
         * \throw bpmodule::exception::OptionException if
         *        the value is invalid (and not expert)
         *
         * \exstrong 
         */
        template<typename T>
        void Change(const std::string & key, const T & value)
        {
            GetOrThrow_Cast_<T>(key)->ChangeValue(value);
        }


        /*! \brief Sets an option to its default
         *
         * \throw bpmodule::exception::OptionException if
         *        the option does not have a default
         *
         * \exstrong 
         */ 
        void ResetToDefault(const std::string & key)
        {
            GetOrThrow_(key)->ResetToDefault();
        }


        /*! \brief Check all options to see if they are valid
         * 
         * \exnothrow
         */
        bool Valid(void) const noexcept
        {
            for(const auto & it : opmap_)
                if(!it.second->Valid())
                    return false;
            return true;
        }



        /*! \brief Test if a given option is valid
         * 
         * \throw bpmodule::exception::MapException if a key doesn't exist
         *
         * \throw bpmodule::exception::PythonConvertException if there
         *        is a problem converting python types.
         */  
        template<typename T>
        bool Validate(const std::string & key, const T & obj) const
        {
            return GetOrThrow_Cast_<T>(key)->Validate(obj);
        }



        /////////////////////////////
        // Python-related functions
        /////////////////////////////
        /*! \brief Construct options from a python dictionary
         * 
         * \throw bpmodule::exception::MapException if there is a duplicate key
         *
         * \throw bpmodule::exception::OptionException if there is
         *        a problem with the option (validation, etc)
         *
         * \throw bpmodule::exception::PythonConvertException if there
         *        is a problem converting python types.
         */ 
        OptionMap(const boost::python::dict & opt);


        /*! \brief Change options via python dictionary
         *
         * Dictionary is simple string key -> value mapping.
         *
         * \throw bpmodule::exception::MapException if a key doesn't exist
         *
         * \throw bpmodule::exception::OptionException if there is
         *        a problem with the option (validation, etc)
         *
         * \throw bpmodule::exception::PythonConvertException if there
         *        is a problem converting python types.
         *
         * \exbasic
         *
         * \note Only offers basic guarantee, but in practice should be strong.
         */
        void ChangePy(const boost::python::dict & opt);



        /*! \brief Change an option by passing a boost::python object
         *
         * \throw bpmodule::exception::MapException if a key doesn't exist
         *
         * \throw bpmodule::exception::OptionException if there is
         *        a problem with the option (validation, etc)
         *
         * \throw bpmodule::exception::PythonConvertException if there
         *        is a problem converting python types.
         * 
         * \exstrong
         */ 
        void ChangePy(const std::string & key, const boost::python::object & obj);



        /*! \brief Return the option's value as a python object
         *
         * \throw bpmodule::exception::MapException if a key doesn't exist
         *
         * \throw bpmodule::exception::PythonConvertException if there
         *        is a problem converting python types.
         *
         * \throw bpmodule::exception::OptionException if the value
         *        does not exist for that option
         */  
        boost::python::object GetPy(const std::string & key) const
        {
            return GetOrThrow_(key)->GetValuePy();
        }



        /*! \brief Validate a python object for an option
         * 
         * \throw bpmodule::exception::MapException if a key doesn't exist
         *
         * \throw bpmodule::exception::PythonConvertException if there
         *        is a problem converting python types.
         */  
        bool ValidatePy(const std::string & key, const boost::python::object & obj) const;


        /*! \brief Validate a python dictionary for an options
         * 
         * \throw bpmodule::exception::MapException if a key doesn't exist
         *
         * \throw bpmodule::exception::PythonConvertException if there
         *        is a problem converting python types.
         */  
        bool ValidatePy(const boost::python::dict & opt) const;



    private:
        std::map<std::string, detail::OptionBasePtr> opmap_;


        /*! \brief Get an OptionBasePtr or throw if the key doesn't exist
         * 
         * \throw bpmodule::exception::MapException if a key doesn't exist
         */
        detail::OptionBasePtr & GetOrThrow_(const std::string & key)
        {
            if(opmap_.count(key))
                return opmap_.at(key);
            else
                throw exception::MapException("Key not found", "OptionMap", key); 
        }



        //! \copydoc GetOrThrow_
        const detail::OptionBasePtr & GetOrThrow_(const std::string & key) const
        {
            if(opmap_.count(key))
                return opmap_.at(key);
            else
                throw exception::MapException("Key not found", "OptionMap", key); 
        }




        /*! \brief Get an OptionBasePtr and cast to an appropriate OptionHolder
         * 
         * \throw bpmodule::exception::MapException if a key doesn't exist or cannot
         *        be cast to the desired type
         */
        template<typename T>
        const detail::OptionHolder<T> * GetOrThrow_Cast_(const std::string & key) const
        {
            const detail::OptionBasePtr & ptr = GetOrThrow_(key);
            const detail::OptionHolder<T> * oh = dynamic_cast<const detail::OptionHolder<T> *>(ptr.get());
            if(oh == nullptr)
                throw exception::MapException("Bad cast", "OptionMap", ptr->Type(), typeid(T).name());

            return oh;
        }
};


} // close namespace datastore
} // close namespace bpmodule



#endif

