/*! \file
 *
 * \brief Storage of cache data
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef BPMODULE_GUARD_DATASTORE__CACHEDATA_HPP_
#define BPMODULE_GUARD_DATASTORE__CACHEDATA_HPP_

#include "bpmodule/exception/Exceptions.hpp"
#include "bpmodule/datastore/GenericHolder.hpp"
#include "bpmodule/python/Convert.hpp"
#include "bpmodule/datastore/OptionMap.hpp"



namespace bpmodule {
namespace datastore {


/*! Storage of cache data
 *
 * Each module gets a CacheData object that is shared between
 * instantiations. It allows for storage of arbitrary data
 * mapped to a key, options pair. The options
 * should be those options used to generate the data but it is generally
 * left up to the module to determine what that means.
 *
 * Functions that take strings for the "significant options"
 * will compare only options with those keys when finding
 * or otherwise manipulating data.
 *
 * Once data is stored, the data itself should be considered
 * constant, although this cannot be strictly enforced
 * in python.
 *
 * \todo thread safety
 * \todo printing
 */
class CacheData
{
    public:
        CacheData(void)          = default;
        virtual ~CacheData(void) = default;


        CacheData(const CacheData &)             = delete;
        CacheData(CacheData &&)                  = default;
        CacheData & operator=(const CacheData &) = delete;
        CacheData & operator=(CacheData &&)      = default;


       /*! \brief Count the number objects stored for a given key
        * 
        * There may be multiple objects stored, since a key can
        * be stored for several options.
        */
        size_t CountKey(const std::string & key) const
        {
            return cmap_.count(key);
        }


        /*! \brief Determine if this object contains data for a key
         *
         * \param [in] key The key to the data
         * \param [in] opt Options associated with the data.
         * \param [in] sigopt Options that are significant for the data
         * \return True if the key exists, false otherwise
         */
        bool HasData(const std::string & key,
                    const OptionMap & opt = OptionMap(),
                    const KeySet & sigopt = KeySet()) const
        {
            auto range = cmap_.equal_range(key);
            for(auto it = range.first; it != range.second; ++it)
                if(opt.CompareSelect(it->second.options, sigopt))
                        return true; 
            return false;
        }



        /*! \brief Determine if this object contains data for a key (python version)
         *
         * \param [in] key The key to the data
         * \param [in] opt Options associated with the data.
         * \param [in] sigopt Options that are significant for the data
         * \return True if the key exists, false otherwise
         */
        bool HasDataPy(const std::string & key,
                      const OptionMap & opt = OptionMap(),
                      pybind11::list sigopt = pybind11::list()) const
        {
            return HasData(key, opt, python::ConvertToCpp<KeySet>(sigopt));
        }




        /*! \brief Obtain all the keys contained in this object
         * 
         * \return A vector of strings containing all the keys
         */
        KeySet GetKeys(void) const
        {
            KeySet v;
            for(auto & it : cmap_)
                v.insert(it.first);
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
            return cmap_.size();
        }




        /*! \brief Return the underlying data
         *
         * \throw bpmodule::exception::DataStoreException if key (with the given
         *        significant options) doesn't exist or is of the wrong type
         *
         * \tparam T The type of the data
         *
         * \param [in] key The key to the data
         * \param [in] opt Options associated with the data.
         * \param [in] sigopt Options that are significant for the data
         * \return A const referance to the data
         */
        template<typename T>
        const T & Get(const std::string & key,
                      const OptionMap & opt = OptionMap(),
                      const KeySet & sigopt = KeySet()) const
        {
            const detail::GenericHolder<T> * ph = GetOrThrow_Cast_<T>(key, opt, sigopt);
            return ph->GetRef();
        }



        /*! \brief Return the underlying data (python version)
         * 
         * Since it actually stores python objects, returning by
         * copy or reference is undefined.
         *
         * \throw bpmodule::exception::DataStoreException if key (with the given
         *        significant options) doesn't exist
         *
         * \tparam T The type of the data
         *
         * \param [in] key The key to the data
         * \param [in] opt Options associated with the data.
         * \param [in] sigopt Options that are significant for the data
         * \return A copy of the data
         */
        pybind11::object GetPy(const std::string & key,
                                   const OptionMap & opt = OptionMap(),
                                   pybind11::list sigopt = pybind11::list()) const
        {
            return Get<pybind11::object>(key, opt,
                                         python::ConvertToCpp<KeySet>(sigopt));
        }



        /*! \brief Add data associated with a given key via copy
         * 
         * Keys are not overwritten. Instead, additional keys are inserted
         *
         * \tparam T The type of the data
         *
         * \param [in] key The key to the data
         * \param [in] opt Options associated with the data.
         * \param [in] value The data to store
         */
        template<typename T>
        void Set(const std::string & key, const T & value,
                 const OptionMap & opt = OptionMap())
        {
            Set_(key, std::unique_ptr<detail::GenericHolder<T>>(new detail::GenericHolder<T>(value)), opt);
        }



        /*! \brief Set the data associated with a given key (python version)
         * 
         * Data is stored as a python object
         * 
         * Keys are not overwritten. Instead, additional keys are inserted
         *
         * \param [in] key The key to the data
         * \param [in] opt Options associated with the data.
         * \param [in] value The data to store
         */
        void SetPy(const std::string & key, pybind11::object value,
                   const OptionMap & opt = OptionMap())
        {
            Set(key, value, opt);
        }



        /*! \brief Add data associated with a given key via move semantics
         * 
         * \copydetails Set
         */
        template<typename T>
        void Take(const std::string & key, T && value,
                  const OptionMap & opt = OptionMap())
        {
            Set_(key, new detail::GenericHolder<T>(value), opt);
        }



        /*! \brief Remove a key from this data store
         * 
         * The key does not have to exist. If the key doesn't exists, nothing will happen.
         *
         * \param [in] key The key to the data
         * \return The number of elements removed
         */
        size_t Erase(const std::string & key)
        {
            return cmap_.erase(key);
        }





    private:
        ////////////////////////////////
        // Actual storage of the data //
        ////////////////////////////////

        /*! \brief Stores a pointer to a placeholder, plus some other information
         */
        struct CacheDataEntry
        {
            OptionMap options;                               //! Options used for the data
            std::unique_ptr<detail::GenericBase> value;      //! The stored data
        };



        //! The container to use to store the data
        std::multimap<std::string, CacheDataEntry> cmap_;




        ////////////////////////////////
        // Private functions          //
        ////////////////////////////////

        /*! \brief Obtains a CacheDataEntry or throws if data doesn't exist
         * 
         * \throw bpmodule::exception::DataStoreException if key (with the given
         *        significant options) doesn't exist
         *
         * \param [in] key Key of the data to get
         * \param [in] opt Options associated with the data.
         * \param [in] sigopt Options that are significant for the data
         * \return CacheDataEntry containing the data for the given key
         */ 
        const CacheDataEntry & GetOrThrow_(const std::string & key,
                                           const OptionMap & opt,
                                           const KeySet & sigopt) const
        {
            if(cmap_.count(key))
            {
                auto range = cmap_.equal_range(key);
                for(auto it = range.first; it != range.second; ++it)
                {
                    if(sigopt.size() == 0 && opt.Compare(it->second.options))
                        return it->second;
                    
                    if(sigopt.size() && opt.CompareSelect(it->second.options, sigopt))
                        return it->second;
                }

                // if you got here, it was not found
                throw exception::DataStoreException("Key with these options not found in CacheData", "key", key);
            }
            else
                throw exception::DataStoreException("Key not found in CacheData", "key", key);
        }




        /*! \brief Obtains a pointer to a GenericHolder cast to desired type
         * 
         * \throw bpmodule::exception::DataStoreException if key (with the given 
         *        significant options) doesn't exist or the cast fails.
         *
         * \param [in] key Key of the data to get
         * \param [in] opt Options associated with the data.
         * \param [in] sigopt Options that are significant for the data
         * \return detail::GenericHolder containing the data for the given key
         */ 
        template<typename T>
        const detail::GenericHolder<T> * GetOrThrow_Cast_(const std::string & key,
                                                          const OptionMap & opt,
                                                          const KeySet & sigopt) const
        {
            const CacheDataEntry & pme = GetOrThrow_(key, opt, sigopt);
            const detail::GenericHolder<T> * ph = dynamic_cast<const detail::GenericHolder<T> *>(pme.value.get());
            if(ph == nullptr)
                throw exception::DataStoreException("Bad cast in CacheData", "key", key,
                                                    "fromtype", pme.value->Type(),
                                                    "totype", typeid(T).name()); 

            return ph;
        }



        /*! \brief Sets the data for a given key via a pointer
         * 
         * \param [in] key Key of the data to set
         * \param [in] value Pointer to the data to set
         * \param [in] opt Options to be associated with the data
         */ 
        void Set_(const std::string & key, std::unique_ptr<detail::GenericBase> && value, const OptionMap & opt)
        {
            // emplace has strong exception guarantee
            cmap_.emplace(key, CacheDataEntry{opt, std::move(value)});
        }


};




} // close namespace datastore
} // close namespace bpmodule



#endif

