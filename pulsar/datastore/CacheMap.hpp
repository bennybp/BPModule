/*! \file
 *
 * \brief Storage of cache data (header)
 */ 


#pragma once

#include <set>
#include <map>
#include <mutex>
#include <thread>

#include <pybind11/pybind11.h>

#include "pulsar/datastore/GenericHolder.hpp"
#include "pulsar/datastore/GenericHolder_serialized.hpp"

namespace pulsar {
class Checkpoint;
}


namespace pulsar {


/*! Storage of cache data
 *
 * This contains all cache data for all modules. The key given to
 * these functions should contain a module key (unique to each module) and
 * some unique string representing the data stored under that key
 *
 * Once data is stored, the data itself should be considered
 * constant, although this cannot be strictly enforced
 * in python.
 *
 * \todo thread safety
 */
class CacheMap
{
    public:

        //! \todo these are unsigned int rather than enum, since
        //        we need to be able to combine them in python
        //        (pybind11 won't be able to do bitwise OR, etc,
        //        unless they are integers)
        static constexpr const unsigned int NoPolicy         = 0;
        static constexpr const unsigned int CheckpointLocal  = 1;
        static constexpr const unsigned int CheckpointGlobal = 2;
        static constexpr const unsigned int DistributeGlobal = 4;


        CacheMap(void);

        virtual ~CacheMap(void) = default;


        CacheMap(const CacheMap &)             = delete;
        CacheMap(CacheMap &&)                  = default;
        CacheMap & operator=(const CacheMap &) = delete;
        CacheMap & operator=(CacheMap &&)      = default;


        /*! \brief Obtain all of the unique keys contained in this object
         * 
         * This only returns the local keys
         */
        std::set<std::string> get_keys(void) const;


        /*! \brief Return the number of elements contained
         * 
         * This only returns the size of the local cache map
         */
        size_t size(void) const noexcept;


        /*! \brief Return the underlying data
         *
         * If the key does not exist or is of the wrong type,
         * an empty shared_ptr is returned.
         *
         * \tparam T The type of the data
         *
         * \param [in] key The key to the data
         * \return A const referance to the data
         */
        template<typename T>
        std::shared_ptr<const T> get(const std::string & key, bool use_distcache)
        {
            static_assert(!std::is_reference<T>::value,
                          "Cannot get/store a reference type in the cache");

            static_assert(!std::is_pointer<T>::value,
                          "Cannot get/store a pointer type in the cache");

            using detail::GenericBase;
            using detail::GenericHolder;
            using detail::SerializedGenericData;
            typedef typename std::remove_cv<T>::type HeldType;
            typedef detail::GenericHolder<HeldType> HolderType;

            std::unique_lock<std::mutex> l(mutex_);

            // Try to obtain the entry in the local cache
            const CacheMapEntry_ * cme = nullptr;

            if(cmap_.count(key))
                cme = &cmap_.at(key);
            else if(use_distcache)
            {
                // See if we can obtain it from the distributed cache.
                // This will place it in the local cache
                l.unlock();
                obtain_from_distcache_(key);
                l.lock();

                if(cmap_.count(key)) 
                    cme = &cmap_.at(key);
            }
            

            if(cme == nullptr) // could not find data
                return {};

            auto ptr = cme->value.get();

            // attempt to cast to a GenericHolder of the correct type
            const HolderType * ph = dynamic_cast<const HolderType *>(ptr);
            if(ph != nullptr) // found it
                return ph->get();  //implicitly cast to std::shared_ptr<const T>

            // If we didn't find it, see if it is serialized data
            const GenericHolder<SerializedGenericData> * sdh;
            sdh = dynamic_cast<GenericHolder<SerializedGenericData> *>(ptr);
            if(sdh == nullptr)
            {
                // We didn't find the correct type nor did we find
                // serialized data
                return {};
            }

            // We found serialized data with that key
            // See if the type is correct
            const std::string heldtype_str = typeid(HeldType).name();
            const std::string serialized_type = sdh->type();

            if(heldtype_str != serialized_type)
                return {}; // We have a serialized entry, but it
                           // doesn't store the correct type
            

            // convert to a new holder
            const SerializedGenericData & sgd = *(sdh->get()); // get() returns a shared_ptr
            std::unique_ptr<HeldType> new_data = sdh->unserialize<HeldType>();
            std::unique_ptr<HolderType> new_entry(new HolderType(std::move(*new_data)));


            // the shared_ptr we are actually returning
            // Why this is done here: We need the shared pointer (with
            // the incremented counter) in case the entry gets erased soon
            // after unlocking the mutex.
            std::shared_ptr<const T> retptr = new_entry->get();

            // actually add to map (this replaces the old data)
            set_(key, std::move(new_entry), sgd.policy);

            // it should be safe now to unlock the mutex
            l.unlock();

            // notify the dist cache
            if(sgd.policy & DistributeGlobal)
                notify_distcache_add_(key);

            return retptr;
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
         * \param [in] policy Checkpointing policy for data
         */
        template<typename T>
        void set(const std::string & key, T && value, unsigned int policy)
        {
            static_assert(!std::is_pointer<T>::value,
                          "Cannot get/store a pointer type in the cache");

            using detail::GenericBase;
            using detail::GenericHolder;
            typedef typename std::remove_reference<T>::type HeldType_base;
            typedef typename std::remove_cv<HeldType_base>::type HeldType;
            typedef detail::GenericHolder<HeldType> HolderType;

            // construct outside of mutex locking
            std::unique_ptr<GenericBase> newdata(new HolderType(std::forward<T>(value)));

            // now lock and set
            {
                std::lock_guard<std::mutex> l(mutex_);
                set_(key, std::move(newdata), policy);
            }

            // notify the dist cache
            if(policy & DistributeGlobal)
                notify_distcache_add_(key);

        }


        /*! \brief Remove a key from this data store
         * 
         * The key does not have to exist. If the key doesn't exists, nothing will happen.
         *
         * \param [in] key The key to the data
         * \return The number of elements removed
         */
        size_t erase(const std::string & key);

        /*! \brief Delete all elements in this cache */
        void clear(void);

        /*! Print information about this cache object
         * 
         * \param [in] os The output stream to print to
         */
        void print(std::ostream & os) const;

        /*! \brief Start synchronization across all ranks
         *
         * \warning Two additional tags will be used (tag+1 and tag+2).
         *          Using these elsewhere will lead to problems.
         *
         * \param [in] tag Which MPI tag to use (will also use tag+1 and tag+2)
         */
        void start_sync(int tag);

        /*! \brief Stop synchronization across all ranks */
        void stop_sync(void);


    private:
        friend class Checkpoint;

        /*! \brief Mutex protecting this object during multi-threaded access */
        mutable std::mutex mutex_;

        /*! \brief Stores a pointer to a placeholder, plus some other information
         */
        struct CacheMapEntry_
        {
            std::unique_ptr<detail::GenericBase> value;  //!< The stored data
            unsigned int policy;                         //!< Policy flags for this entry 
        };

        //! The container to use to store the data
        std::map<std::string, CacheMapEntry_> cmap_;


        ////////////////////////////////
        // Private functions          //
        ////////////////////////////////


        /*! \brief sets the data for a given key via a pointer
         * 
         * \param [in] key Key of the data to set
         * \param [in] value Pointer to the data to set
         */ 
        void set_(const std::string & key,
                  std::unique_ptr<detail::GenericBase> && ptr,
                  unsigned int policy)
        {
            if(cmap_.count(key))
                cmap_.erase(key);

            cmap_.emplace(key, CacheMapEntry_{std::move(ptr), policy,});
        }


        ///@{ \name Distributed synchronization of the cache

        //! \brief Allows only one thread to use the sync facility at a time
        std::mutex sync_comm_mutex_;

        /*! \brief The separate synchronization thread */
        std::thread sync_thread_;

        /*! \brief The current MPI tag being used to sync this manager */
        int sync_tag_;

        /*! \brief Function run by the synchronization thread */
        void sync_thread_func_(void);

        /*! \brief Obtain data from another rank, if possible */
        void obtain_from_distcache_(const std::string & key);

        void notify_distcache_add_(const std::string & key);

        void notify_distcache_delete_(const std::string & key);

        ///@}

};




} // close namespace pulsar


