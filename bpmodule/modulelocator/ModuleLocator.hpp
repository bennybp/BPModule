/*! \file
 *
 * \brief Main module database class (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_MODULELOCATOR_HPP_
#define _GUARD_MODULELOCATOR_HPP_

#include <unordered_map>
#include <atomic>

#include "bpmodule/datastore/GraphNodeData.hpp"
#include "bpmodule/datastore/CacheData.hpp"
#include "bpmodule/exception/ModuleCreateException.hpp"
#include "bpmodule/exception/ModuleLocatorException.hpp"
#include "bpmodule/modulelocator/CreatorFunctions.hpp"
#include "bpmodule/modulelocator/ModulePtr.hpp"


// forward declarations
namespace bpmodule {

namespace modulebase {
class ModuleBase;
}

namespace modulelocator {

template<typename T>
class ModuleLoaderBase;

class CModuleLoader;
class PyModuleLoader;
}
}
// end forward declarations

namespace bpmodule {
namespace modulelocator {




/*! \brief Module database
 *
 * Holds loaded modules for later use
 */
class ModuleLocator
{
    public:
        ModuleLocator();
        ~ModuleLocator();


        // no copy construction or assignment
        ModuleLocator(const ModuleLocator & rhs)             = delete;
        ModuleLocator(ModuleLocator && rhs)                  = delete;
        ModuleLocator & operator=(const ModuleLocator & rhs) = delete;
        ModuleLocator & operator=(ModuleLocator && rhs)      = delete;


        /*! \brief Returns the number of modules in the database
         *
         * \exnothrow
         */
        size_t Size(void) const noexcept;


        /*! \brief Returns the keys for all modules in the database
         */
        std::vector<std::string> GetKeys(void) const;


        /*! \brief Returns the information about a module with a given key
         *
         * \throw bpmodule::exception::ModuleLocatorException
         *        if the key doesn't exist in the database
         *
         * \param [in] key A module key
         */
        ModuleInfo KeyInfo(const std::string & key) const;


        
        /*! \brief Prints all the information about the loaded modules
         */
        void PrintInfo(void) const;


        /*! \brief Returns true if a module with the given key exists in the database
         *
         * \param [in] key A module key
         * \return True if the key exists in the map, false if it doesn't
         */
        bool Has(const std::string & key) const;



        /*! \brief Set the options for a module (python version)
         *
         * The dictionary has strings for keys and arbitrary data types for
         * the values
         *
         * \throw bpmodule::exception::ModuleLocatorException
         *        if the key doesn't exist in the database
         *
         * \throw bpmodule::exception::OptionException
         *        if there is a problem converting the options
         *        or if it is invalid
         *
         * \exstrong
         *
         *
         * \param [in] key A module key
         * \param [in] opt Options to set
         */
        void SetOptions(const std::string & key, pybind11::dict opt);




        /*! \brief Test the creation of all modules
         *
         * This goes through and tests the module creation for all keys. It does
         * not attempt to cast them, though. This is a simple sanity check
         *
         * \throw bpmodule::exception::ModuleCreateException if there is a problem
         *
         * \exbasic
         * \todo make strong?
         */
        void TestAll(void);



        /*! \brief Return a new module object wrapped in an RAII-style scoping object
         *
         * \throw bpmodule::exception::ModuleLocatorException
         *        if the key doesn't exist in the database
         *
         * \throw bpmodule::exception::ModuleCreateException if there are other
         *        problems creating the module
         *
         * \exbasic
         * \todo make strong?
         *
         * \param [in] key A module key
         *
         * \return A ScopedModule for an object of the requested type
         */
        template<typename T>
        ModulePtr<T> GetModule(const std::string & key, unsigned long parentid)
        {
            // may throw
            std::unique_ptr<detail::ModuleIMPLHolder> umbptr = CreateModule_(key, parentid);

            // create the ModulePtr type
            ModulePtr<T> mod(std::move(umbptr));


            /*
            T * dptr = dynamic_cast<T *>(mod.first);
            if(dptr == nullptr)
            {
                ModuleInfo mi = store_.at(key).mi;

                throw exception::ModuleCreateException("Bad cast for module", mi.path,
                                                       key, mi.name, 
                                                       "fromtype", util::DemangleCppType(mod.first),
                                                       "totype", util::DemangleCppType<T *>());
            }

            ScopedModule<T> ret(dptr, mod.second); // construction shouldn't throw?
            */

            return mod;
        }


        /*! \brief Retrieve a module as a python object
         * 
         * \throw bpmodule::exception::ModuleLocatorException
         *        if the key doesn't exist in the database
         *
         * \throw bpmodule::exception::ModuleCreateException if there are other
         *        problems creating the module
         *
         * \exbasic
         * \todo make strong?
         *
         * \return The module wrapped in a python object
         */
        pybind11::object GetModulePy(const std::string & key, unsigned long parentid);




        /*! \brief Clears all entries in the cache and performs some cleanup
         */
        void ClearCache(void);




    protected:
        template<typename T>
        friend class ModuleLoaderBase;

        friend class CModuleLoader;
        friend class PyModuleLoader;


        /*! \brief Adds/inserts a module creator to the database
         *
         * \throw bpmodule::exception::ModuleLoaderException if the key
         *        already exists in the database
         *
         * \exstrong
         *
         * \param [in] key A module key
         * \param [in] func A function that generates the module
         * \param [in] dfunc A function that deletes the module
         * \param [in] mi Information about the module
         */
        void InsertModule(const std::string & key, const ModuleCreatorFunc & mc, const ModuleInfo & mi);


    private:

        /*! \brief An entry for a module in the database
         */
        struct StoreEntry
        {
            ModuleInfo mi;             //!< Information for this module
            ModuleCreatorFunc mc;
        };


        /*! \brief Actual storage object - maps keys to creation functions
         */
        std::unordered_map<std::string, StoreEntry> store_;


        /*! \brief Map for storing created module information
         *
         * \todo will be replaced by a graph or tree
         */
        std::unordered_map<unsigned long, datastore::GraphNode> graphnodes_;


        //! The id to assign to the next created module
        std::atomic<unsigned long> curid_;


        /*! \brief Map of cache data
         *
         * The key is a combination of the module name and version
         *
         * \todo is this enough to guarantee uniqueness?
         */
        std::unordered_map<std::string, datastore::CacheData> cachemap_;


        /*! \brief Obtain a module or throw exception
         *
         * \throw bpmodule::exception::ModuleLocatorException
         *        if the key doesn't exist
         *
         * \param [in] key A module key
         */
        const StoreEntry & GetOrThrow_(const std::string & key) const;



        /*! \brief Obtain a module or throw exception
         *
         * \throw bpmodule::exception::ModuleLocatorException
         *        if the key doesn't exist
         *
         * \param [in] key A module key
         */
        StoreEntry & GetOrThrow_(const std::string & key);


        /*! \brief Create a module and its deleter functor
         * 
         * \throw bpmodule::exception::ModuleLocatorException
         *        if the key doesn't exist in the database
         *
         * \throw bpmodule::exception::ModuleCreateException if there are other
         *        problems creating the module
         *
         * \return A pair with the first member being a raw pointer and
         *         the second member being its deleter func
         * 
         * \exbasic
         *
         * \note The calling function is responsible for managing the pointer
         *
         */
        std::unique_ptr<detail::ModuleIMPLHolder>
        CreateModule_(const std::string & key, unsigned long parentid);

};



} // close namespace modulelocator
} // close namespace bpmodule


#endif
