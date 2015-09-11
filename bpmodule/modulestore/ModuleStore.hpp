/*! \file
 *
 * \brief Main module database class (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_MODULESTORE_HPP_
#define _GUARD_MODULESTORE_HPP_

#include <unordered_map>
#include <atomic>

#include "bpmodule/modulestore/ModuleInfo.hpp"

// forward declarations
namespace bpmodule {
namespace modulebase {
class ModuleBase;
}

namespace modulestore {

template<typename T>
class ModuleLoaderBase;

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
        typedef std::function<modulebase::ModuleBase *(const std::string &, unsigned long, ModuleStore &, const ModuleInfo &)> ModuleGeneratorFunc;


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
         */ 
        std::vector<std::string> GetKeys(void) const;


        /*! \brief Returns the information for a given module key
         * 
         * \throw bpmodule::exception::GeneralException if the key doesn't
         *        exist
         *
         * \param [in] key A module key
         */
        ModuleInfo KeyInfo(const std::string & key) const;


        /*! \brief Returns true if the given key exists
         *
         * \param [in] key A module key
         * \return True if the key exists in the map, false if it doesn't
         */ 
        bool Has(const std::string & key) const;



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


        /*! \brief Set the options for a module (python version)
         *
         * \throw bpmodule::exception::GeneralException if the key doesn't
         *        exist
         *
         * \exstrong 
         * 
         * \param [in] key A module key
         * \param [in] opt Options to set
         */
        void SetOptions(const std::string & key, const boost::python::list & opt);



        /*! \brief Return a module wrapped in an RAII-style scoping object
            *
         * \throw bpmodule::exception::GeneralException if the key doesn't
         *        exist or the module cannot be cast to the requested type
         *
         * \exstrong 
         *
         * \param [in] key A module key
         *
         * \return A ScopedModule for an object of the requested type
         */ 
        template<typename T>
        ScopedModule<T> GetModule(const std::string & key)
        {
            T & mod = GetModuleRef_<T>(key);

            // make the deleter function the DeleteModule_() of this ModuleStore object
            std::function<void(modulebase::ModuleBase *)> dfunc = std::bind(static_cast<void(ModuleStore::*)(modulebase::ModuleBase *)>(&ModuleStore::DeleteModule_),
                                                                            this,
                                                                            std::placeholders::_1);

            return ScopedModule<T>(&mod, dfunc);
        }





    protected:
        template<typename T>
        friend class ModuleLoaderBase;

        /*! \brief Adds a module to the map
         *
         * \throw bpmodule::exception::GeneralException if the key already exists
         *
         * \exstrong
         * 
         * \param [in] key A module key
         * \param [in] func A function that generates the module
         * \param [in] dfunc A function that deletes the module
         * \param [in] minfo Information about the module
         */ 
        void AddModule(const std::string & key, ModuleGeneratorFunc func,
                       ModuleRemoverFunc dfunc, const ModuleInfo & minfo);


    private:

        /*! \brief An entry for a module in the database
         */ 
        struct StoreEntry
        {
            ModuleInfo mi;             //!< Information for this module
            ModuleGeneratorFunc func;  //!< Function that creates a class from this module
            ModuleRemoverFunc dfunc;   //!< Function that deletes a class from this module
        };

        /*! \brief The type of the actual storage map
         */
        typedef std::unordered_map<std::string, StoreEntry> StoreMap;


        /*! \brief The type of the map that stores removal functions for a given id
         */
        typedef std::unordered_map<unsigned long, ModuleRemoverFunc> RemoverMap;


        /*! \brief Actual storage object
         */ 
        StoreMap store_;


        /*! \brief Map for storing object removal information
         */ 
        RemoverMap removemap_;


        //! The id to assign to the next created module
        std::atomic<unsigned long> curid_;


        /*! \brief Obtain a module or throw exception
         * 
         * This function can be const since the database itself does not
         * change at this point.
         *
         * \throw bpmodule::exception::GeneralException if the key doesn't
         *        exist
         *
         * \param [in] key A module key
         */ 
        const StoreEntry & GetOrThrow_(const std::string & key) const;


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
        T & GetModuleRef_(const std::string & key)
        {
            // obtain the creator
            const StoreEntry & se = GetOrThrow_(key);

            // create
            modulebase::ModuleBase * mbptr = se.func(key, curid_, *this, se.mi);

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
            removemap_.emplace(curid_, se.dfunc);

            // next id
            curid_++;

            return *dptr;
        }



        /*! \brief Removes a created module object from storage
         * 
         * This actually deletes the object, and removes references
         * to it in various places.
         *
         * If the id doesn't exist, nothing will happen.
         *
         * \exsafe If an exception is thrown, the module instance is still
         *         removed from this database.
         *
         * \param [in] mb Pointer to the module to remove
         */ 
        void DeleteModule_(modulebase::ModuleBase * mb);


        /*! \brief Removes a created module object from storage
         * 
         * This actually deletes the object, and removes references
         * to it in various places.
         *
         * If the id doesn't exist, nothing will happen.
         *
         * \exsafe If an exception is thrown, the module instance is still
         *         removed from this database.
         *
         * \param [in] id ID of the module to remove
         */ 
        void DeleteModule_(long id);

};



} // close namespace modulestore
} // close namespace bpmodule


#endif
