/*! \file
 *
 * \brief Storage of generic data via (key, value) pair (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_PROPERTYMAP_HPP_
#define _GUARD_PROPERTYMAP_HPP_

#include <map>

#include "bpmodule/exception/DataStoreException.hpp"
#include "bpmodule/datastore/GenericHolder.hpp"

namespace bpmodule {
namespace datastore {


/*! \brief A mapping of arbirtrary data with some copy-on-write semantics
 *
 * A PropertyMap can store an arbitrary data type, as long that data type
 * has a copy and move constructor. Once stored, the data itself is immutable.
 *
 * Multiple PropertyMap can store references to the same data.
 * Since the data itself is read-only, modifications in one map cannot affect
 * another. Erasing or replacing the data in one map leaves the references
 * in any other maps referencing this data untouched.
 *
 * \tparam KEYTYPE the type of key to use in the map.
 */
template<typename KEYTYPE>
class PropertyMap
{
    public:
        PropertyMap(void)          = default;
        virtual ~PropertyMap(void) = default;


        /*! \brief Copies a property map
         * 
         * Underlying data is NOT copied, rather just the references.
         *
         * \param [in] rhs The object to copy
         */
        PropertyMap(const PropertyMap & rhs)             = default;



        /*! \brief Moves the contents of another object to this
         * 
         * References to data are removed from the other object.
         *
         * \param [in] rhs The object to copy
         */ 
        PropertyMap(PropertyMap && rhs)                  = default;



        /*! \brief Assigns the contents of another object to this
         *
         * \copydetails PropertyMap::PropertyMap(const PropertyMap &)
         */ 
        PropertyMap & operator=(const PropertyMap & rhs) = default;



        /*! \brief Moves the contents of another object to this
         *
         * \copydetails PropertyMap::PropertyMap(PropertyMap &&)
         */ 
        PropertyMap & operator=(PropertyMap && rhs)      = default;



        /*! \brief Determine if this object contains data for a key
         *
         * \param [in] key The key to the data
         * \return True if the key exists, false otherwise
         */
        bool HasKey(const KEYTYPE & key) const
        {
            return opmap_.count(key);
        }



        /*! \brief Determine if this object contains data of a specific type for a key
         *
         * \tparam T Type to compare to
         *
         * \param [in] key The key to the data
         * \return True if the key exists and is of type T, false otherwise
         */
        template<typename T>
        bool HasType(const KEYTYPE & key) const
        {
            if(!HasKey(key))
                return false;

            // craziest syntax
            return GetOrThrow_(key).value->template IsType<T>();
        }



        /*! \brief Get a string representing the type for a given key
         *
         * \throw bpmodule::exception::DataStoreException
         *        if the key doesn't exist 
         *
         * \param [in] key The key to the data
         * \return A string representing the type for a key
         */
        std::string GetType(const KEYTYPE & key) const
        {
            return GetOrThrow_(key).value->Type();
        }



        /*! \brief Get a string representing the type for a given key (demangled)
         *
         * \throw bpmodule::exception::DataStoreException
         *        if the key doesn't exist
         *
         * \param [in] key The key to the data
         * \return A string representing the type for a key
         */
        std::string GetDemangledType(const KEYTYPE & key) const
        {
            return GetOrThrow_(key).value->DemangledType();
        }




        /*! \brief Obtain all the keys contained in this object
         * 
         * \return A vector of strings containing all the keys
         */
        std::vector<KEYTYPE> GetKeys(void) const
        {
            std::vector<std::string> v;
            v.reserve(opmap_.size());
            for(auto & it : opmap_)
                v.push_back(it.first);
            return v;
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



    protected:
        /////////////////////////////////////////////////
        // to be exposed selectively by derived classes
        /////////////////////////////////////////////////

        /*! \brief Return a const reference to the underlying data
         *
         * \throw bpmodule::exception::DataStoreException
         *        if the key doesn't exist or 
         *        is of the wrong type
         *
         * \tparam T The type of the data
         *
         * \param [in] key The key to the data
         * \return A const referance to the data
         */
        template<typename T>
        const T & GetRef(const KEYTYPE & key) const
        {
            const detail::GenericHolder<T> * ph = GetOrThrow_Cast_<T>(key);
            return ph->GetRef();
        }



        /*! \brief Return a copy of the underlying data
         *
         * \throw bpmodule::exception::DataStoreException
         *        if the key doesn't exist or 
         *        is of the wrong type
         *
         * \tparam T The type of the data
         *
         * \param [in] key The key to the data
         * \return A copy of the data
         */
        template<typename T>
        T GetCopy(const KEYTYPE & key) const
        {
            return GetRef<T>(key);
        }





        /*! \brief Set the data associated with a given key via copy
         * 
         * If the key exists, the data is overwritten.
         * Then, the data itself may remain if another PropertyMap contains the same data
         * (set via SetRef, etc).
         *
         * \exstrong
         *
         * \tparam T The type of the data
         *
         * \param [in] key The key to the data
         * \param [in] value The data to store
         */
        template<typename T>
        void Set(const KEYTYPE & key, const T & value)
        {
            Set_(key, detail::GenericBasePtr(new detail::GenericHolder<T>(value)));
        }



        /*! \brief Set the data associated with a given key via move semantics
         * 
         * \copydetails Set
         */
        template<typename T>
        void Take(const KEYTYPE & key, T && value)
        {
            Set_(key, detail::GenericBasePtr(new detail::GenericHolder<T>(value)));
        }



        /*! \brief Store the same data as found in another object
         * 
         * This function does not copy or take the actual data. Rather, only a
         * pointer is copied. Erasing or replacing data in this map does not
         * affect the other map. If the key already exists in this map, it is
         * replaced.
         *
         * \throw bpmodule::exception:DataStoreException if the key is not found in the other map
         *
         * \exstrong
         *
         * \param [in] other The other object from which to get the data
         * \param [in] key The key of the object in the other object
         */
        void SetRef(const PropertyMap<KEYTYPE> & other, const KEYTYPE & key)
        {
            //Setref with the same key
            SetRef(other, key, key);
        }



        /*! \copydoc SetRef
         * \param [in] newkey The key under which to store the data in this object
         */
        void SetRef(const PropertyMap<KEYTYPE> & other, const KEYTYPE & key, const KEYTYPE & newkey)
        {
            // get the shared ptr, etc, from the other property map
            // This should copy the shared_ptr
            PropMapEntry pe = other.GetOrThrow_(key);
        
            // add it here
            // should have strong exception guarantee
            if(opmap_.count(newkey))
                opmap_.at(newkey) = pe;
            else
                opmap_.emplace(newkey, pe);
        }



        /*! \brief Remove a key from this data store
         * 
         * The data itself may remain if another PropertyMap contains the same data
         * (set via SetRef, etc).
         *
         * The key does not have to exist. If the key doesn't exists, nothing will happen.
         *
         * \exstrong
         *
         * \param [in] key The key to the data
         * \return The number of elements removed
         */
        size_t Erase(const KEYTYPE & key)
        {
            return opmap_.erase(key);
        }



        ///////////////////////////////////
        // Python functions
        ///////////////////////////////////

        /*! \copydoc GetCopy
         *
         * Returns the data as a python object. 
         *
         * \throw bpmodule::exception::PythonConvertException if there is a problem with a conversion
         */
        boost::python::object GetCopyPy(const KEYTYPE & key) const
        {
            // may throw
            return GetOrThrow_(key).value->GetPy();
        }


        /*! \copydoc Set
         *
         * Copies the internal python data to a C++ type.
         *
         * \throw bpmodule::exception::PythonConvertException if there is a problem with a conversion
         */
        void SetPy(const KEYTYPE & key, const boost::python::object & value)
        {
            // catch errors from conversions in GenericHolderFactory
            try {
                Set_(key, detail::GenericHolderFactory(value));
            }
            catch(exception::DataStoreException & ex)
            {
                // append key info if GenericHolderFactory throws
                ex.AppendInfo("mapkey", key);
                throw;
            }
        }




    private:


        ////////////////////////////////
        // Actual storage of the data //
        ////////////////////////////////

        /*! \brief Stores a pointer to a placeholder, plus some other information
         */
        struct PropMapEntry
        {
            detail::GenericBasePtr value;      //! The stored data
        };



        //! The container to use to store the data
        typedef std::map<std::string, PropMapEntry> PropMap;


        //! Map actually containing the data
        PropMap opmap_;




        ////////////////////////////////
        // Private functions          //
        ////////////////////////////////

        /*! \brief Obtains a PropMapEntry or throws if key doesn't exist
         * 
         * \throw bpmodule::exception::DataStoreException if key doesn't exist
         *
         * \exstrong
         *
         * \param [in] key Key of the data to get
         * \return PropMapEntry containing the data for the given key
         */ 
        PropMapEntry & GetOrThrow_(const KEYTYPE & key)
        {
            if(opmap_.count(key))
                return opmap_.at(key);
            else
                throw exception::DataStoreException("Key not found", key,
                                                    "location", "PropertyMap");
        }



        //! \copydoc GetOrThrow_
        const PropMapEntry & GetOrThrow_(const KEYTYPE & key) const
        {
            if(opmap_.count(key))
                return opmap_.at(key);
            else
                throw exception::DataStoreException("Key not found", key,
                                                    "location", "PropertyMap");
        }




        /*! \brief Obtains a pointer to a GenericHolder cast to desired type
         * 
         * \throw bpmodule::exception::DataStoreException if key 
         *        doesn't exist or if the cast fails.
         *
         * \param [in] key Key of the data to get
         * \return detail::GenericHolder containing the data for the given key
         */ 
        template<typename T>
        const detail::GenericHolder<T> * GetOrThrow_Cast_(const KEYTYPE & key) const
        {
            const PropMapEntry & pme = GetOrThrow_(key);
            const detail::GenericHolder<T> * ph = dynamic_cast<const detail::GenericHolder<T> *>(pme.value.get());
            if(ph == nullptr)
                throw exception::DataStoreException("Bad cast", key,
                                                    "location", "PropertyMap",
                                                    "fromtype", pme.value->Type(),
                                                    "totype", typeid(T).name()); 

            return ph;
        }



        /*! \brief Sets the data for a given key via a GenericBasePtr
         * 
         * \exstrong
         *
         * \param [in] key Key of the data to set
         * \param [in] value Pointer to the data to set
         */ 
        void Set_(const KEYTYPE & key, detail::GenericBasePtr && value)
        {
            if(opmap_.count(key))
            {
                PropMapEntry & phe = GetOrThrow_(key);
                phe.value = value;
            }
            else
            {
                // emplace has strong exception guarantee
                opmap_.emplace(key, PropMapEntry{value});
            }
        }




};




} // close namespace datastore
} // close namespace bpmodule



#endif

