/*! \file
 *
 * \brief Storage of cache data (header)
 */ 


#pragma once

#include "pulsar/datastore/CacheMap.hpp"

namespace pulsar {
namespace datastore {


/*! Storage of cache data
 *
 * Each module gets a CacheData object that is shared between
 * instantiations. This contains a pointer to a master
 * CacheMap object which actually stores the data.
 *
 * Once data is stored, the data itself should be considered
 * constant, although this cannot be strictly enforced
 * in python.
 */
class CacheData
{
    public:
        static const unsigned int NoPolicy;
        static const unsigned int CheckpointLocal;
        static const unsigned int CheckpointGlobal;
        static const unsigned int DistributeGlobal;

        CacheData(CacheMap * parent_cmap, std::string module_key)
            : module_key_(module_key + "%%"),  // use %% as a separator
              parent_cmap_(parent_cmap)
        { }

        virtual ~CacheData(void) = default;


        CacheData(const CacheData &)             = delete;
        CacheData(CacheData &&)                  = default;
        CacheData & operator=(const CacheData &) = delete;
        CacheData & operator=(CacheData &&)      = default;


        /*! \brief Obtain all the keys contained in this object
         * 
         * \return A vector of strings containing all the keys
         */
        std::set<std::string> get_keys(void) const;

        /*! \brief Return the number of elements contained
         *
         * \exnothrow
         *
         * \return Number of elements in this container
         */
        size_t size(void) const noexcept;

        /*! \brief Return the underlying data
         *
         * \throw pulsar::DataStoreException if key
         *        doesn't exist or is of the wrong type
         *
         * \tparam T The type of the data
         *
         * \param [in] key The key to the data
         * \return A const referance to the data
         */
        template<typename T>
        std::shared_ptr<const T> get(const std::string & key, bool use_distcache)
        {
            return parent_cmap_->get<T>(make_full_key_(key), use_distcache);
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
            const std::string full_key = make_full_key_(key);
            parent_cmap_->set(full_key, std::forward<T>(value), policy);
        }

        /*! \brief Remove a key from this data store
         * 
         * The key does not have to exist. If the key doesn't exists, nothing will happen.
         *
         * \param [in] key The key to the data
         * \return The number of elements removed
         */
        size_t erase(const std::string & key);


    private:
        //! A unique string representing the module that this belongs to
        std::string module_key_;

        //! Parent CacheMap object
        CacheMap * parent_cmap_;
        

        std::string make_full_key_(const std::string & data_key) const
        {
            return module_key_ + data_key;
        }

        /*! \brief Obtains all the keys from the parent that belong to this module */
        std::set<std::string> get_my_keys_(void) const
        {
            std::set<std::string> my_keys;
            auto all_keys = parent_cmap_->get_keys();
            for(auto & k : all_keys)
            {
                // only gdo greate than, since the actual data key must be at
                // least one character
                if(k.size() > module_key_.size() &&
                   k.substr(0, module_key_.size()) == module_key_)
                    my_keys.emplace(std::move(k));
            }

            return my_keys;
        }

};


} // close namespace datastore
} // close namespace pulsar

