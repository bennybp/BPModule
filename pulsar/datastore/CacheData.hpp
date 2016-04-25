/*! \file
 *
 * \brief Storage of cache data (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef PULSAR_GUARD_DATASTORE__CACHEDATA_HPP_
#define PULSAR_GUARD_DATASTORE__CACHEDATA_HPP_

#include <set>
#include <map>
#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/output/Output.hpp"
#include "pulsar/datastore/GenericHolder.hpp"
#include "pulsar/python/Pybind11.hpp"



namespace pulsar{
namespace datastore {


/*! Storage of cache data
 *
 * Each module gets a CacheData object that is shared between
 * instantiations. It allows for storage of arbitrary data
 * mapped to a key. What the key represents is left up to
 * the module.
 *
 * Once data is stored, the data itself should be considered
 * constant, although this cannot be strictly enforced
 * in python.
 *
 * \todo thread safety
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


        /*! \brief Determine if this object contains data for a key
         *
         * \param [in] key The key to the data
         * \return True if the key exists, false otherwise
         */
        bool Count(const std::string & key) const
        {
            return cmap_.count(key);
        }

        /*! \brief Obtain all the keys contained in this object
         * 
         * \return A vector of strings containing all the keys
         */
        std::set<std::string> GetKeys(void) const
        {
            std::set<std::string> v;
            for(const auto & it : cmap_)
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
         * \throw pulsar::exception::DataStoreException if key
         *        doesn't exist or is of the wrong type
         *
         * \tparam T The type of the data
         *
         * \param [in] key The key to the data
         * \return A const referance to the data
         */
        template<typename T>
        const T & Get(const std::string & key) const
        {
            const detail::GenericHolder<T> * ph = GetOrThrow_Cast_<T>(key);
            return ph->GetRef();
        }

        /*! \brief Return the underlying data (python version)
         * 
         * Since it actually stores python objects, returning by
         * copy or reference is undefined.
         *
         * \throw pulsar::exception::DataStoreException if key
         *        doesn't exist
         *
         * \tparam T The type of the data
         *
         * \param [in] key The key to the data
         * \return A copy of the data
         */
        pybind11::object GetPy(const std::string & key) const
        {
            return Get<pybind11::object>(key);
        }

        /*! \brief Add data associated with a given key via copy
         * 
         * Keys are not overwritten. Instead, additional keys are inserted
         *
         * \tparam T The type of the data
         *
         * \param [in] key The key to the data
         * \param [in] value The data to store
         */
        template<typename T>
        void Set(const std::string & key, const T & value)
        {
            Set_(key, std::unique_ptr<detail::GenericBase>(new detail::GenericHolder<T>(value)));
        }

        /*! \brief Add data associated with a given key via move semantics
         * 
         * \copydetails Set
         */
        template<typename T>
        void Take(const std::string & key, T && value)
        {
            Set_(key, std::unique_ptr<detail::GenericBase>(new detail::GenericHolder<T>(std::move(value))));
        }

        /*! \brief Set the data associated with a given key (python version)
         * 
         * Data is stored as a python object
         * 
         * Keys are not overwritten. Instead, additional keys are inserted
         *
         * \param [in] key The key to the data
         * \param [in] value The data to store
         */
        void SetPy(const std::string & key, const pybind11::object & value)
        {
            Set(key, value);
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

        /*! \brief Print information about the cache
         */
        void Print(std::ostream & os) const
        {
            using namespace pulsar::output;

            Output(os, "Cache data with %? entries\n", Size());

            for(const auto & it : cmap_)
                Output(os, "  -Key: %-20?  Type: %?\n", it.first,
                       it.second.value->DemangledType());
        }


    private:
        ////////////////////////////////
        // Actual storage of the data //
        ////////////////////////////////

        /*! \brief Stores a pointer to a placeholder, plus some other information
         * 
         * May be expanded in the future
         */
        struct CacheDataEntry
        {
            std::unique_ptr<detail::GenericBase> value;      //! The stored data
        };



        //! The container to use to store the data
        std::map<std::string, CacheDataEntry> cmap_;




        ////////////////////////////////
        // Private functions          //
        ////////////////////////////////

        /*! \brief Obtains a CacheDataEntry or throws if data doesn't exist
         * 
         * \throw pulsar::exception::DataStoreException if key
         *        doesn't exist
         *
         * \param [in] key Key of the data to get
         * \return CacheDataEntry containing the data for the given key
         */ 
        const CacheDataEntry & GetOrThrow_(const std::string & key) const
        {

            if(cmap_.count(key))
                return cmap_.at(key);
            else
                throw exception::DataStoreException("Key not found in CacheData", "key", key);
        }


        /*! \brief Obtains a pointer to a GenericHolder cast to desired type
         * 
         * \throw pulsar::exception::DataStoreException if key
         *        doesn't exist or the cast fails.
         *
         * \param [in] key Key of the data to get
         * \return detail::GenericHolder containing the data for the given key
         */ 
        template<typename T>
        const detail::GenericHolder<T> * GetOrThrow_Cast_(const std::string & key) const
        {
            const CacheDataEntry & pme = GetOrThrow_(key);
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
         */ 
        void Set_(const std::string & key, std::unique_ptr<detail::GenericBase> && ptr)
        {
            // emplace has strong exception guarantee
            cmap_.emplace(key, CacheDataEntry{std::move(ptr)});
        }


};




} // close namespace datastore
} // close namespace pulsar



#endif

