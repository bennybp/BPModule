/*! \file
 *
 * \brief Storage of cache data (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#pragma once

#include <set>
#include <map>
#include <mutex>
#include <thread>

#include <pybind11/pybind11.h>

#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/output/Output.hpp"
#include "pulsar/datastore/GenericHolder.hpp"

namespace pulsar {
namespace modulemanager {
class Checkpoint;
}
}


namespace pulsar {
namespace datastore {


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
        static constexpr const unsigned int NoCheckpoint     = 1;
        static constexpr const unsigned int CheckpointLocal  = 2;
        static constexpr const unsigned int CheckpointGlobal = 4;
        static constexpr const unsigned int DistributeGlobal = 8;


        CacheMap(void)
            : sync_tag_(-1) { }

        virtual ~CacheMap(void) = default;


        CacheMap(const CacheMap &)             = delete;
        CacheMap(CacheMap &&)                  = default;
        CacheMap & operator=(const CacheMap &) = delete;
        CacheMap & operator=(CacheMap &&)      = default;


        /*! \brief Determine if this object contains data
         *
         * \param [in] module_key The key to the data
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
        std::shared_ptr<const T> get(const std::string & key)
        {
            const detail::GenericHolder<T> * ph;

            {
                std::lock_guard<std::mutex> l(mutex_);
                ph = get_or_throw_cast_<T>(key);
            }

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
        pybind11::object get_py(const std::string & key)
        {
            const detail::GenericHolder<pybind11::object> * ph;

            {
                std::lock_guard<std::mutex> l(mutex_);
                ph = get_or_throw_cast_py_(key);
            }

            return *(ph->get());
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
         * \param [in] policyflags Checkpointing policy for data
         */
        template<typename T>
        void set(const std::string & key, T && value, unsigned int policyflags)
        {
            typedef typename std::remove_cv<typename std::remove_reference<T>::type>::type HolderType;

            // construct outside of mutex locking
            std::unique_ptr<detail::GenericBase> newdata(new detail::GenericHolder<HolderType>(std::forward<T>(value)));

            // now lock and set
            std::lock_guard<std::mutex> l(mutex_);
            set_(key, std::move(newdata), policyflags);
        }


        /*! \brief Set the data associated with a given key (python version)
         * 
         * Data is stored as a python object
         * 
         * \param [in] key The key to the data
         * \param [in] value The data to store
         * \param [in] policyflags Checkpointing policy for data
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


        void clear(void)
        {
            std::lock_guard<std::mutex> l(mutex_);
            cmap_.clear();
        }


        void print(std::ostream & os) const
        {
            using namespace pulsar::output;

            // We lock the mutex here. Therefore, we cannot call CacheMap::size(), etc.
            std::lock_guard<std::mutex> l(mutex_);
 
            print_output(os, "Cache data with %? entries\n", cmap_.size());

            for(const auto & it : cmap_)
                print_output(os, "  -Key: %-20?  Serializable: %?  Type: %?\n", it.first,
                             it.second.value->is_serializable() ? "Yes" : "No",
                             it.second.value->demangled_type());
        }


        /*! \brief Start synchronization across all ranks */
        void start_sync(int tag);

        /*! \brief Stop synchronization across all ranks */
        void stop_sync(void);


    private:
        friend class modulemanager::Checkpoint;

        /*! \brief Mutex protecting this object during multi-threaded access */
        mutable std::mutex mutex_;

        /*! \brief The separate synchronization thread */
        std::thread sync_thread_;

        /*! \brief The current MPI tag being used to sync this manager */
        int sync_tag_;

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

        /*! \brief Obtains a CacheMapEntry_ or throws if data doesn't exist
         * 
         * \throw pulsar::exception::DataStoreException if key
         *        doesn't exist
         *
         * \param [in] key Key of the data to get
         * \return CacheMapEntry_ containing the data for the given key
         */ 
        const CacheMapEntry_ & get_or_throw_(const std::string & key) const
        {
            if(cmap_.count(key))
                return cmap_.at(key);
            else
                throw exception::DataStoreException("Key not found in CacheMap", "key", key);
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
        const detail::GenericHolder<T> * get_or_throw_cast_(const std::string & key)
        {
            //////////////////////////////////////////////////////
            // This whole function is to be called from a public function, which
            // should lock this structure for the entire duration
            //////////////////////////////////////////////////////

            // may be needed for unserializing
            typedef typename std::remove_cv<typename std::remove_reference<T>::type>::type HolderType;

            using namespace detail;

            const CacheMapEntry_ & pme = get_or_throw_(key);
            auto ptr = pme.value.get();

            const GenericHolder<T> * ph = dynamic_cast<const GenericHolder<T> *>(ptr);
            if(ph == nullptr)
            {
                // is this serialized data?
                const SerializedDataHolder * sdh = dynamic_cast<const SerializedDataHolder *>(ptr);
                if(sdh == nullptr)
                {
                    throw exception::DataStoreException("Bad cast in CacheMap", "key", key,
                                                        "fromtype", pme.value->demangled_type(),
                                                        "totype", util::demangle_cpp_type<T>());
                }
                else
                {
                    // convert to a new holder
                    std::unique_ptr<T> unserialized(sdh->get<T>());
                    unsigned int policyflags = sdh->policy_flags();

                    // will replace the old key

                    // this is basically a copy of set(), but we need to avoid a mutex deadlock
                    std::unique_ptr<detail::GenericBase> newdata(new detail::GenericHolder<HolderType>(std::move(*unserialized)));

                    set_(key, std::move(newdata), policyflags);

                    // call this function again - should load the unserialized data
                    return get_or_throw_cast_<T>(key);
                }
            }

            return ph;
        }

        /*! \brief Obtains a pointer to a GenericHolder cast to desired type
         * 
         * \throw pulsar::exception::DataStoreException if key
         *        doesn't exist or the cast fails.
         *
         * \param [in] key Key of the data to get
         * \return detail::GenericHolder containing the data for the given key
         */ 
        const detail::GenericHolder<pybind11::object> * get_or_throw_cast_py_(const std::string & key)
        {
            // we need a separate function since pybind11::object is not serializable.
            //! \todo fix if we can serialize pybind11::objects
            using namespace detail;

            const CacheMapEntry_ & pme = get_or_throw_(key);
            auto ptr = pme.value.get();

            const GenericHolder<pybind11::object> * ph = dynamic_cast<const GenericHolder<pybind11::object> *>(ptr);

            if(ph == nullptr)
            {
                    throw exception::DataStoreException("Bad cast in CacheMap", "key", key,
                                                        "fromtype", pme.value->demangled_type(),
                                                        "totype", util::demangle_cpp_type<pybind11::object>());
            }

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
            if(cmap_.count(key))
                cmap_.erase(key);
            cmap_.emplace(key, CacheMapEntry_{std::move(ptr), policyflags,});
        }

        /*! \brief Function run by the synchronization thread */
        void sync_thread_func_(void);
};




} // close namespace datastore
} // close namespace pulsar


