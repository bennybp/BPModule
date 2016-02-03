/*! \file
 *
 * \brief Main module database class (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_MODULEMANAGER__MODULEMANAGER_HPP_
#define BPMODULE_GUARD_MODULEMANAGER__MODULEMANAGER_HPP_

#include <unordered_map>
#include <atomic>

#include "bpmodule/datastore/ModuleGraph.hpp"
#include "bpmodule/datastore/CacheData.hpp"
#include "bpmodule/exception/Exceptions.hpp"
#include "bpmodule/modulemanager/ModuleCreationFuncs.hpp"
#include "bpmodule/modulemanager/ModulePtr.hpp"



namespace bpmodule {
namespace modulemanager {


// forward declaration
class ModuleLoaderBase;



/*! \brief Module database
 *
 * Holds loaded modules for later use
 */
class ModuleManager
{
    public:
        ModuleManager();
        ~ModuleManager();


        // no copy construction or assignment
        ModuleManager(const ModuleManager & rhs)             = delete;
        ModuleManager(ModuleManager && rhs)                  = delete;
        ModuleManager & operator=(const ModuleManager & rhs) = delete;
        ModuleManager & operator=(ModuleManager && rhs)      = delete;


        /*! \brief Returns the number of modules in the database
         *
         * \exnothrow
         */
        size_t Size(void) const noexcept;


        /*! \brief Returns the information about a module with a given module key
         *
         * \throw bpmodule::exception::ModuleManagerException
         *        if the module key or its associated name doesn't exist in the database
         *
         * \param [in] modulekey A module key
         */
        ModuleInfo ModuleKeyInfo(const std::string & modulekey) const;


        /*! \brief Returns the information about a module with a given module name
         *
         * \throw bpmodule::exception::ModuleManagerException
         *        if the module name doesn't exist in the database
         *
         * \param [in] modulekey A module key
         */
        ModuleInfo ModuleNameInfo(const std::string & modulename) const;


        
        /*! \brief Prints all the information about the loaded modules
         */
        void PrintInfo(void) const;


        /*! \brief Returns true if a module with the given module key exists in the database
         *
         * \param [in] modulekey A module key
         * \return True if the key exists in the map, false if it doesn't
         */
        bool HasKey(const std::string & modulekey) const;


        /*! \brief Returns true if a module with the given module name exists in the database
         *
         * \param [in] modulename Name of the module
         * \return True if the name exists in the map, false if it doesn't
         */
        bool HasName(const std::string & modulename) const;



        /*! \brief Associates a key with a given module name
         * 
         * \throw bpmodule::exception::ModuleManagerException
         *        if the module key already exists or if a module with the given
         *        name doesn't exist
         *
         * \param [in] modulekey The key to be used
         * \param [in] modulename The name of the module to be associated with \p key
         */
        void AddKey(const std::string & modulekey, const std::string & modulename);


        /*! \brief Associates or re-associates a key with a given module name
         * 
         * Will overwrite if the key already exists
         *
         * \throw bpmodule::exception::ModuleManagerException
         *        if a module with the given name doesn't exist
         *
         * \param [in] modulekey The key to be used
         * \param [in] modulename The name of the module to be associated with \p key
         */
        void ReplaceKey(const std::string & modulekey, const std::string & modulename);




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
         * \throw bpmodule::exception::ModuleManagerException
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
         * \throw bpmodule::exception::ModuleManagerException
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



        /*! \brief Change an option for a module
         */
        template<typename T>        
        void ChangeOption(const std::string & modulekey, const std::string & optkey, const T & value)
        {
            GetOrThrow_(modulekey).mi.options.Change(optkey, value);
        }



        /*! \brief Change an option via python
         */
        void ChangeOptionPy(const std::string & modulekey, const std::string & optkey, const pybind11::object & value);



        /*! \brief Return the graph in DOT format
         */ 
        std::string DotGraph(void) const;



        /*! \brief Adds/inserts a module creator to the database
         *
         * \throw bpmodule::exception::ModuleLoaderException if the key
         *        already exists in the database or if \p mc doesn't
         *        contain a creator for the given module name (in \p mi)
         *
         *  \note We pass all module creation funcs. This is so we
         *        don't need to export IMPL holders to pybind11 
         *
         * \param [in] mc Functions for creating modules
         * \param [in] mi Information about the module
         */
        void LoadModuleFromModuleInfo(const ModuleInfo & minfo);



    private:

        /*! \brief An entry for a module in the database
         */
        struct StoreEntry
        {
            ModuleInfo mi;             //!< Information for this module
            ModuleCreationFuncs::Func mc; //!< Function to create this module
        };


        /*! \brief Handlers for different module types
         */   
        std::unordered_map<std::string, std::unique_ptr<ModuleLoaderBase>> loadhandlers_;


        /*! \brief Actual storage object - maps module names to creation functions
         */
        std::unordered_map<std::string, StoreEntry> store_;


        /*! \brief Stores map of keys to module names
         */
        std::unordered_map<std::string, std::string> keymap_;


        /*! \brief Map for storing created module information
         *
         * \todo will be replaced by a pointer to the graph
         */
        datastore::ModuleGraph mgraph_;


        /*! \brief Map of module ID to graph nodes
         *
         * \todo replace with something?
         */
        std::map<unsigned long, datastore::ModuleGraphNode> mgraphmap_;


        //! The id to assign to the next created module
        std::atomic<unsigned long> curid_;


        /*! \brief Map of cache data
         *
         * The key is a combination of the module name and version
         */
        std::unordered_map<std::string, datastore::CacheData> cachemap_;


        /*! \brief Obtain the module name for a key or throw an exception
         * 
         * \throw bpmodule::exception::ModuleManagerException
         *        if the key doesn't exist
         */
        std::string GetOrThrowKey_(const std::string & modulekey) const;


        /*! \brief Obtain stored internal info for a module (via name) or throw an exception
         *
         * \throw bpmodule::exception::ModuleManagerException
         *        if the name doesn't exist
         *
         * \param [in] modulename Name of the module
         */
        const StoreEntry & GetOrThrowName_(const std::string & modulename) const;


        /*! \copydoc GetOrThrowName_
         */ 
        StoreEntry & GetOrThrowName_(const std::string & modulename);


        /*! \brief Obtain stored internal info for a module (via key) or throw an exception
         *
         * \throw bpmodule::exception::ModuleManagerException
         *        if the key or name doesn't exist
         *
         * \param [in] modulekey A module key
         */
        const StoreEntry & GetOrThrow_(const std::string & modulekey) const;


        /*! \copydoc GetOrThrow_
         */
        StoreEntry & GetOrThrow_(const std::string & modulekey);



        /*! \brief Create a module via its creator function
         * 
         * \throw bpmodule::exception::ModuleManagerException
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



} // close namespace modulemanager
} // close namespace bpmodule


#endif
