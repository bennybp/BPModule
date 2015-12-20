/*! \file
 *
 * \brief Storage of cache data
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef BPMODULE_GUARD_DATASTORE__CACHEDATA_HPP_
#define BPMODULE_GUARD_DATASTORE__CACHEDATA_HPP_

#include <map>

#include "bpmodule/exception/Exceptions.hpp"
#include "bpmodule/datastore/GenericHolder.hpp"
#include "bpmodule/python/Convert.hpp"
#include "bpmodule/datastore/OptionMap.hpp"

//! \todo split out python stuff?


namespace bpmodule {
namespace datastore {

//! \todo docs outdated
//! \todo printing
class CacheData
{
    public:
        CacheData(void)          = default;
        virtual ~CacheData(void) = default;


        CacheData(const CacheData & rhs)             = delete;
        CacheData(CacheData && rhs)                  = default;
        CacheData & operator=(const CacheData & rhs) = delete;
        CacheData & operator=(CacheData && rhs)      = default;

        size_t CountKey(const std::string & key) const
        {
            return cmap_.count(key);
        }


        /*! \brief Determine if this object contains data for a key
         *
         * \param [in] key The key to the data
         * \return True if the key exists, false otherwise
         */
        bool HasKey(const std::string & key,
                    const OptionMap & opt = OptionMap(),
                    const std::vector<std::string> & sigopt = std::vector<std::string>()) const
        {
            auto range = cmap_.equal_range(key);
            for(auto it = range.first; it != range.second; ++it)
                if(opt.CompareSelect(it->second.options, sigopt))
                        return true; 
            return false;
        }


        bool HasKeyPy(const std::string & key,
                      const OptionMap & opt = OptionMap(),
                      pybind11::list sigopt = pybind11::list()) const
        {
            return HasKey(key, opt, python::ConvertToCpp<std::vector<std::string>>(sigopt));
        }




        /*! \brief Obtain all the keys contained in this object
         * 
         * \return A vector of strings containing all the keys
         */
        std::vector<std::string> GetKeys(void) const
        {
            std::vector<std::string> v;
            v.reserve(cmap_.size());
            for(auto & it : cmap_)
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
            return cmap_.size();
        }




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
        const T & GetRef(const std::string & key,
                         const OptionMap & opt = OptionMap(),
                         const std::vector<std::string> & sigopt = std::vector<std::string>()) const
        {
            const detail::GenericHolder<T> * ph = GetOrThrow_Cast_<T>(key, opt, sigopt);
            return ph->GetRef();
        }


        pybind11::object GetRefPy(const std::string & key,
                                  const OptionMap & opt = OptionMap(),
                                  pybind11::list sigopt = pybind11::list()) const
        {
            return GetRef<pybind11::object>(key, opt,
                                            python::ConvertToCpp<std::vector<std::string>>(sigopt));
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
        T GetCopy(const std::string & key, const OptionMap & opt,
                  const std::vector<std::string> & sigopt) const
        {
            return GetRef<T>(key, opt, sigopt);
        }



        pybind11::object GetCopyPy(const std::string & key,
                                   const OptionMap & opt = OptionMap(),
                                   pybind11::list sigopt = pybind11::list()) const
        {
            return GetRef<pybind11::object>(key, opt,
                                            python::ConvertToCpp<std::vector<std::string>>(sigopt));
        }


        /*! \brief Set the data associated with a given key via copy
         * 
         * If the key exists, the data is overwritten.
         * Then, the data itself may remain if another CacheData contains the same data
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
        void Set(const std::string & key, const T & value,
                 const OptionMap & opt = OptionMap())
        {
            Set_(key, std::unique_ptr<detail::GenericHolder<T>>(new detail::GenericHolder<T>(value)), opt);
        }



        void SetPy(const std::string & key, pybind11::object value,
                   const OptionMap & opt = OptionMap())
        {
            Set(key, value, opt);
        }



        /*! \brief Set the data associated with a given key via move semantics
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
         * The data itself may remain if another CacheData contains the same data
         * (set via SetRef, etc).
         *
         * The key does not have to exist. If the key doesn't exists, nothing will happen.
         *
         * \exstrong
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
            std::unique_ptr<detail::GenericBase> value;      //! The stored data
            OptionMap options;        //! Options used for the data
        };



        //! The container to use to store the data
        std::multimap<std::string, CacheDataEntry> cmap_;




        ////////////////////////////////
        // Private functions          //
        ////////////////////////////////

        /*! \brief Obtains a CacheDataEntry or throws if key doesn't exist
         * 
         * \throw bpmodule::exception::DataStoreException if key doesn't exist
         *
         * \exstrong
         *
         * \param [in] key Key of the data to get
         * \return CacheDataEntry containing the data for the given key
         */ 
        CacheDataEntry & GetOrThrow_(const std::string & key,
                                     const OptionMap & opt,
                                     const std::vector<std::string> & sigopt)
        {
            if(cmap_.count(key))
            {
                auto range = cmap_.equal_range(key);
                for(auto it = range.first; it != range.second; ++it)
                {
                    if(sigopt.size())
                    {
                        if(opt.CompareSelect(it->second.options, sigopt))
                            return it->second;
                    }
                    else
                    {
                        if(opt.Compare(it->second.options))
                            return it->second;
                    }
                }

                throw exception::DataStoreException("Key with these options not found in CacheData", "key", key);
            }
            else
                throw exception::DataStoreException("Key not found in CacheData", "key", key);
        }



        //! \copydoc GetOrThrow_
        const CacheDataEntry & GetOrThrow_(const std::string & key,
                                           const OptionMap & opt,
                                           const std::vector<std::string> & sigopt) const
        {
            if(cmap_.count(key))
            {
                auto range = cmap_.equal_range(key);
                for(auto it = range.first; it != range.second; ++it)
                {
                    if(sigopt.size())
                    {
                        if(opt.CompareSelect(it->second.options, sigopt))
                            return it->second;
                    }
                    else
                    {
                        if(opt.Compare(it->second.options))
                            return it->second;
                    }
                }

                throw exception::DataStoreException("Key with these options not found in CacheData", "key", key);
            }
            else
                throw exception::DataStoreException("Key not found in CacheData", "key", key);
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
        const detail::GenericHolder<T> * GetOrThrow_Cast_(const std::string & key,
                                                          const OptionMap & opt,
                                                          const std::vector<std::string> & sigopt) const
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
         * \exstrong
         *
         * \param [in] key Key of the data to set
         * \param [in] value Pointer to the data to set
         */ 
        void Set_(const std::string & key, std::unique_ptr<detail::GenericBase> && value, const OptionMap & opt)
        {
            // emplace has strong exception guarantee
            cmap_.emplace(key, CacheDataEntry{std::move(value), opt});
        }


};




} // close namespace datastore
} // close namespace bpmodule



#endif

