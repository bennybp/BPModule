/*! \file
 *
 * \brief Output and printing functionality (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_MODULESTORE_HPP_
#define _GUARD_MODULESTORE_HPP_

#include <unordered_map>
#include <vector>
#include <atomic>

#include "bpmodule/modulestore/ModuleInfo.hpp"

// forward declarations
namespace bpmodule {
namespace modulebase {
class ModuleBase;
}
}
// end forward declarations

namespace bpmodule {
namespace modulestore {

//! Contains a module that automatically deletes the object when leaving scope
template<typename T>
using ScopedModule = std::unique_ptr<T, std::function<void(modulebase::ModuleBase *)>>;


/*! \brief Holds loaded modules
 */
class ModuleStore
{
    public:
        //! A function that generates a module derived from ModuleBase
        typedef std::function<modulebase::ModuleBase *(const std::string &, unsigned long, const ModuleInfo &)> ModuleGeneratorFunc;

        //! A function that deletes a module (by id)
        typedef std::function<void(unsigned long)> ModuleRemoverFunc;

        ModuleStore();
        ~ModuleStore();

        // no copy construction or assignment
        ModuleStore(const ModuleStore & rhs)             = delete;
        ModuleStore(ModuleStore && rhs)                  = delete;
        ModuleStore & operator=(const ModuleStore & rhs) = delete;
        ModuleStore & operator=(ModuleStore && rhs)      = delete;

        /*! \brief Returns the number of loaded modules
         *
         * \exnothrow
         */ 
        size_t Size(void) const noexcept;


        /*! \brief Returns the keys for all loaded modules
         *
         * \exstrong
         */ 
        std::vector<std::string> GetKeys(void) const;


        /*! \brief Returns the information for a given module key
         * 
         * \throw bpmodule::exception::GeneralException if the key doesn't
         *        exist
         *
         * \exstrong
         *
         */
        ModuleInfo KeyInfo(const std::string & key) const;


        /*! \brief Returns true if the given key exists
         *
         * \exstrong
         *
         * \param [in] key A module key
         * \return True if the key exists in the map, false if it doesn't
         */ 
        bool Has(const std::string & key) const;


        //! \todo Add variant of Has that checks type also


        /*! \brief Adds a module to the map
         *
         * \exstrong
         * 
         * \param [in] key A module key
         * \param [in] func A function that generates the module
         * \param [in] dfunc A function that deletes the module
         * \param [in] minfo Information about the module
         */ 
        void AddModule(const std::string & key, ModuleGeneratorFunc func, ModuleRemoverFunc dfunc, const ModuleInfo & minfo);


        /*! \brief Removes a module from the map
         *
         * If the id doesn't exist, nothing will happen.
         *
         * \exbasic
         * \todo Really only a basic guarentee? Depends on the deleterfunc?
         *
         * \param [in] id The id of the module
         */ 
        void RemoveModule(unsigned long id);


        /*! \brief Removes a module from the map
         *
         * If the key doesn't exist, nothing will happen.
         *
         * \exbasic
         * \todo Really only a basic guarentee? Depends on the deleterfunc?
         *
         * \param [in] mb Pointer to the module to remove
         */ 
        void RemoveModule(modulebase::ModuleBase * mb);


        /*! \brief Set the options for a module
         *
         * \throw bpmodule::exception::GeneralException if the key doesn't
         *        exist
         *
         * \exstrong 
         * 
         * \param [in] key A module key
         * \param [in] opt Options to set
         */
        void SetOptions(const std::string & key, const datastore::OptionMap & opt);



        /*! \brief Obtain a module
         *
         * \throw bpmodule::exception::GeneralException if the key doesn't
         *        exist or the module cannot be cast to the requested type
         *
         * \exstrong 
         * 
         * \param [in] key A module key
         *
         * \return A reference to an module object of the requested type
         */
        template<typename T>
        T & GetModule(const std::string & key)
        {
            // obtain the creator
            const StoreEntry & se = GetOrThrow_(key);

            // create
            modulebase::ModuleBase * mbptr = se.func(key, curid_, se.mi);

            // test
            T * dptr = dynamic_cast<T *>(mbptr);
            if(dptr == nullptr)
                throw exception::GeneralException(
                                 "Bad cast for module",
                                 {
                                     { "Location", "ModuleStore"},
                                     { "Key", key },
                                     { "From", typeid(mbptr).name() },
                                     { "To", typeid(T *).name() }
                                 }
                               );


            // store the deleter
            removemap_.insert(RemoverMap::value_type(curid_, se.dfunc));


            // next id
            curid_++;

            return *dptr;
        }


        template<typename T>
        ScopedModule<T> GetScopedModule(const std::string & key)
        {
            T & mod = GetModule<T>(key);
            std::function<void(modulebase::ModuleBase *)> dfunc = std::bind(static_cast<void(ModuleStore::*)(modulebase::ModuleBase *)>(&ModuleStore::RemoveModule),
                                                                this,
                                                                std::placeholders::_1);

            return ScopedModule<T>(&mod, dfunc);
        }



    private:

        struct StoreEntry
        {
            ModuleInfo mi;
            ModuleGeneratorFunc func;
            ModuleRemoverFunc dfunc;
        };

        typedef std::unordered_map<std::string, StoreEntry> StoreMap;
        typedef std::unordered_map<unsigned long, ModuleRemoverFunc> RemoverMap;

        StoreMap store_;
        RemoverMap removemap_;
        std::atomic<unsigned long> curid_;

        const StoreEntry & GetOrThrow_(const std::string & key) const;
};



} // close namespace modulestore
} // close namespace bpmodule


#endif
