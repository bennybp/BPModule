/*! \file
 *
 * \brief Storage of options for a module (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_OPTIONMAP_HPP_
#define _GUARD_OPTIONMAP_HPP_

#include <map>

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


        OptionMap(const OptionMap & rhs)
        {
            for(const auto & it : rhs.opmap_)
                opmap_.emplace(it.first, detail::OptionBasePtr(it.second->Clone()));
        }


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


        /*! \brief Construct options from a python object
         *
         * \copydetails PropertyMap::PropertyMap(const boost::python::list &)
         */ 
        //OptionMap(const boost::python::list & olist) : PropertyMap(olist) {  }


        // throw mapexception or optionexception
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
            return opmap_.at(key)->Has();
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


        bool IsDefault(const std::string & key) const
        {
            return GetOrThrow_(key)->IsDefault();
        }


        //! \todo check if strong exception
        template<typename T>
        void Change(const std::string & key, const T & value)
        {
            GetOrThrow_Cast_<T>(key)->ChangeValue(value);
        }


        void ResetToDefault(const std::string & key)
        {
            GetOrThrow_(key)->ResetToDefault();
        }


    private:
        std::map<std::string, detail::OptionBasePtr> opmap_;


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

