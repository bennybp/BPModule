/*! \file
 *
 * \brief Storage of cache data (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef PULSAR_GUARD_DATASTORE__CACHEDATA_HPP_
#define PULSAR_GUARD_DATASTORE__CACHEDATA_HPP_

#include <set>
#include <map>
#include <mutex>
#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/output/Output.hpp"
#include "pulsar/datastore/GenericHolder.hpp"
#include <pybind11/pybind11.h>

namespace pulsar {
namespace modulemanager {
class Checkpoint;
}
}


namespace pulsar {
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
 * \todo note why we don't allow setting via shared_ptr (data in cache is immutable)
 */
class CacheData
{
    public:

        enum CachePolicy
        {
            NoCheckpoint     = 1,
            CheckpointLocal  = 2,
            CheckpointGlobal = 4
        };


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
        bool count(const std::string & key) const
        {
            std::lock_guard<std::mutex> l(mutex_);
            return cmap_.count(key);
        }

        /*! \brief Obtain all the keys contained in this object
         * 
         * \return A vector of strings containing all the keys
         */
        std::set<std::string> get_keys(void) const
        {
            std::set<std::string> v;

            std::lock_guard<std::mutex> l(mutex_);
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
        size_t size(void) const noexcept
        {
            std::lock_guard<std::mutex> l(mutex_);
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
        std::shared_ptr<const T> get(const std::string & key) const
        {
            // mutex locking handled in get_or_throw_cast
            const detail::GenericHolder<T> * ph = get_or_throw_cast_<T>(key);
            return ph->get();
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
        pybind11::object get_py(const std::string & key) const
        {
            return *(get<pybind11::object>(key));
        }

        /*! \brief Add data associated with a given key via copy
         * 
         * Keys are overwritten if they exist. This should not affect any data
         * previously retrieved via get(), etc
         *
         * \tparam T The type of the data
         *
         * \param [in] key The key to the data
         * \param [in] value The data to store
         */
        template<typename T>
        void set(const std::string & key, T && value, unsigned int policyflags)
        {
            typedef typename std::remove_cv<typename std::remove_reference<T>::type>::type HolderType;

            // construct outside of mutex locking
            std::unique_ptr<detail::GenericBase> newdata(new detail::GenericHolder<HolderType>(std::forward<T>(value)));

            // mutex locking handled in set_
            set_(key, std::move(newdata), policyflags);
        }


        /*! \brief Set the data associated with a given key (python version)
         * 
         * Data is stored as a python object
         * 
         * \param [in] key The key to the data
         * \param [in] value The data to store
         */
        void set_py(const std::string & key, const pybind11::object & value, unsigned int policyflags)
        {
            set(key, value, policyflags);
        }

        /*! \brief Remove a key from this data store
         * 
         * The key does not have to exist. If the key doesn't exists, nothing will happen.
         *
         * \param [in] key The key to the data
         * \return The number of elements removed
         */
        size_t erase(const std::string & key)
        {
            std::lock_guard<std::mutex> l(mutex_);
            return cmap_.erase(key);
        }

        void print(std::ostream & os) const
        {
            using namespace pulsar::output;

            // We lock the mutex here. As such, we cannot call size(), etc.
            std::lock_guard<std::mutex> l(mutex_);
 
            print_output(os, "Cache data with %? entries\n", cmap_.size());

            for(const auto & it : cmap_)
                print_output(os, "  -Key: %-20?  Serializable: %?  Type: %?\n", it.first,
                             it.second.value->is_serializable() ? "Yes" : "No",
                             it.second.value->demangled_type());
        }


    private:
        friend class modulemanager::Checkpoint;

        ////////////////////////////////
        // Actual storage of the data //
        ////////////////////////////////

        /*! \brief Mutex protecting this object during multi-threaded access */
        mutable std::mutex mutex_;

        /*! \brief ID to give to the next piece of data coming in */
        uint64_t curid_ = 0;


        /*! \brief Stores a pointer to a placeholder, plus some other information
         */
        struct CacheDataEntry
        {
            std::unique_ptr<detail::GenericBase> value;  //!< The stored data
            unsigned int policy;                         //!< Policy flags for this entry 
            uint64_t uid;                                //!< Unique ID given to the data
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
        const CacheDataEntry & get_or_throw_(const std::string & key) const
        {
            std::lock_guard<std::mutex> l(mutex_);

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
        const detail::GenericHolder<T> * get_or_throw_cast_(const std::string & key) const
        {
            // get_or_throw will lock the mutex and then release it when done
            const CacheDataEntry & pme = get_or_throw_(key);

            // this stuff is safe to do with an unlocked mutex
            const detail::GenericHolder<T> * ph = dynamic_cast<const detail::GenericHolder<T> *>(pme.value.get());
            if(ph == nullptr)
                throw exception::DataStoreException("Bad cast in CacheData", "key", key,
                                                    "fromtype", pme.value->demangled_type(),
                                                    "totype", util::demangle_cpp_type<T>()); 

            return ph;
        }


        /*! \brief sets the data for a given key via a pointer
         * 
         * \param [in] key Key of the data to set
         * \param [in] value Pointer to the data to set
         */ 
        void set_(const std::string & key,
                  std::unique_ptr<detail::GenericBase> && ptr,
                  unsigned int policyflags)
        {
            std::lock_guard<std::mutex> l(mutex_);

            // emplace has strong exception guarantee
            cmap_.emplace(key, CacheDataEntry{std::move(ptr), policyflags, curid_});
            curid_++;
        }


};




} // close namespace datastore
} // close namespace pulsar



#endif

