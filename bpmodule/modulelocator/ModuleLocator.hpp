/*! \file
 *
 * \brief Main module database class (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_MODULELOCATOR__MODULELOCATOR_HPP_
#define BPMODULE_GUARD_MODULELOCATOR__MODULELOCATOR_HPP_

#include <unordered_map>
#include <atomic>

#include "bpmodule/datastore/GraphNodeData.hpp"
#include "bpmodule/datastore/CacheData.hpp"
#include "bpmodule/exception/Exceptions.hpp"
#include "bpmodule/modulelocator/ModuleCreationFuncs.hpp"
#include "bpmodule/modulelocator/ModulePtr.hpp"


// forward declarations
namespace bpmodule {
namespace modulelocator {
class CppModuleLoader;

namespace export_python {
extern "C" PyObject * PyInit_modulelocator(void);
}
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
        std::vector<std::string> GetModuleKeys(void) const;


        /*! \brief Returns the information about a module with a given module key
         *
         * \throw bpmodule::exception::ModuleLocatorException
         *        if the module key doesn't exist in the database
         *
         * \param [in] modulekey A module key
         */
        ModuleInfo ModuleKeyInfo(const std::string & modulekeykey) const;


        
        /*! \brief Prints all the information about the loaded modules
         */
        void PrintInfo(void) const;


        /*! \brief Returns true if a module with the given module key exists in the database
         *
         * \param [in] modulekey A module key
         * \return True if the key exists in the map, false if it doesn't
         */
        bool Has(const std::string & modulekey) const;




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
         * \param [in] modulekey A module key
         *
         * \return A ScopedModule for an object of the requested type
         */
        template<typename T>
        ModulePtr<T> GetModule(const std::string & modulekey, unsigned long parentid)
        {
            // may throw
            std::unique_ptr<detail::ModuleIMPLHolder> umbptr = CreateModule_(modulekey, parentid);

            if(!umbptr->IsType<T>())
                throw exception::ModuleCreateException("Module for this key is not of the right type",
                                                       "modulekey", modulekey,
                                                       "expectedtype", util::DemangleCppType<T>());

            // create the ModulePtr type
            ModulePtr<T> mod(std::move(umbptr));

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
        pybind11::object GetModulePy(const std::string & modulekey, unsigned long parentid);




        /*! \brief Clears all entries in the cache and performs some cleanup
         * 
         * Must be run before unloading SOs
         */
        void ClearCache(void);


        /*! \brief Clears all entries in the module store
         *
         * Must be run before unloading SOs
         */
        void ClearStore(void);





        /*! \brief Change an option for a module
         */
        template<typename T>        
        void ChangeOption(const std::string & modulekey, const std::string & optkey, const T & value)
        {
            GetOrThrow_(modulekey).mi.options.Change(optkey, value);
        }



        /*! \brief Change an option via python
         */
        void ChangeOptionPy(const std::string & modulekey, const std::string & optkey, pybind11::object value);



    protected:
        friend class CppModuleLoader;
        friend PyObject * export_python::PyInit_modulelocator(void);

        /*! \brief Adds/inserts a module creator to the database
         *
         * \throw bpmodule::exception::ModuleLoaderException if the key
         *        already exists in the database
         *
         * \param [in] modulekey A module key
         * \param [in] func A function that generates the module
         * \param [in] dfunc A function that deletes the module
         * \param [in] mi Information about the module
         */
        void InsertModule(const ModuleCreationFuncs & cf, const ModuleInfo & mi);


    private:

        /*! \brief An entry for a module in the database
         */
        struct StoreEntry
        {
            ModuleInfo mi;             //!< Information for this module
            ModuleCreationFuncs::Func mc; //!< Function to create this module
        };


        /*! \brief Actual storage object - maps module keys to creation functions
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
         */
        std::unordered_map<std::string, datastore::CacheData> cachemap_;


        /*! \brief Obtain a module or throw exception
         *
         * \throw bpmodule::exception::ModuleLocatorException
         *        if the key doesn't exist
         *
         * \param [in] modulekey A module key
         */
        const StoreEntry & GetOrThrow_(const std::string & modulekey) const;



        /*! \copydoc GetOrThrow_
         */
        StoreEntry & GetOrThrow_(const std::string & modulekey);


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
        CreateModule_(const std::string & modulekey, unsigned long parentid);

};



} // close namespace modulelocator
} // close namespace bpmodule


#endif
