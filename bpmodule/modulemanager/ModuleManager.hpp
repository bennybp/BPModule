/*! \file
 *
 * \brief Main module database class (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_MODULEMANAGER__MODULEMANAGER_HPP_
#define BPMODULE_GUARD_MODULEMANAGER__MODULEMANAGER_HPP_

#include <atomic>

#include "bpmodule/datastore/CacheData.hpp"
#include "bpmodule/exception/Exceptions.hpp"
#include "bpmodule/modulemanager/ModuleCreationFuncs.hpp"
#include "bpmodule/modulemanager/ModuleTree.hpp"
#include "bpmodule/modulemanager/ModulePtr.hpp"



namespace bpmodule {
namespace modulemanager {


// forward declaration
class SupermoduleLoaderBase;



/*! \brief Handles loading of supermodules and creation of modules
 *
 * This is the workhorse of the project. It is used to load supermodules,
 * manage the module names and keys, and to create modules via those
 * names and keys.
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
         * \param [in] modulename A module name
         */
        ModuleInfo ModuleNameInfo(const std::string & modulename) const;



        /*! \brief Prints all the information about the loaded modules
         */
        void Print(std::ostream & os) const;


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
         * \param [in] modulename The name of the module to be associated with \p modulekey
         */
        void AddKey(const std::string & modulekey, const std::string & modulename);


        /*! \brief Associates or re-associates a key with a given module name
         *
         * Will overwrite if the key already exists. If it doesn't exist, it will
         * be added.
         *
         * \throw bpmodule::exception::ModuleManagerException
         *        if a module with the given name doesn't exist
         *
         * \param [in] modulekey The key to be used
         * \param [in] modulename The name of the module to be associated with \p modulekey
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



        /*! \brief Create a new module as a wrapped C++ object
         *
         * \throw bpmodule::exception::ModuleManagerException
         *        if the key doesn't exist in the database
         *
         * \throw bpmodule::exception::ModuleCreateException if there are other
         *        problems creating the module
         *
         * \exbasic
         * \todo make strong?
         * \todo might be interesting to make extern template at some point
         *
         * \param [in] modulekey A module key
         * \param [in] parentid ID of the parent module (that this will be a child of)
         *
         * \return A wrapped C++ object of the requested type
         */
        template<typename T>
        ModulePtr<T> GetModule(const std::string & modulekey, ID_t parentid)
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
        pybind11::object GetModulePy(const std::string & modulekey, ID_t parentid);



        /*! \brief Check to see if a module is currently in use
         */
        bool ModuleInUse(ID_t id) const;


        /*! \brief Notify the manager that a module is being destructed
         *
         * \warning Only to be called from the ModuleBase destructor
         */
        void DestructionNotify(ID_t id) noexcept;





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



        /*! \brief Adds/inserts a module creator to the database
         *
         * The supermodule is loaded via a handler, and then info
         * for the module is extracted from the supermodule information.
         *
         * \throw bpmodule::exception::ModuleLoaderException if the key
         *        already exists in the database or if \p mc doesn't
         *        contain a creator for the given module name (in \p mi)
         *
         *  \note We pass all module creation funcs. This is so we
         *        don't need to export IMPL holders to pybind11
         *
         * \param [in] minfo Information about the module
         */
        void LoadModuleFromModuleInfo(const ModuleInfo & minfo);


        /*! \brief Enable debugging for a specific key
         *
         * The key doesn't have to exist -- it will be used if it is ever loaded
         */
        void EnableDebug(const std::string & modulekey, bool debug);


        /*! \brief Enable debug printing on all modules
         */
        void EnableDebugAll(bool debug) noexcept;


        /*! \brief Begin iterating over the module tree
         */
        ModuleTree::const_iterator TreeBegin(ID_t startid) const;

        ModuleTree::const_iterator TreeEnd(void) const;

        ModuleTree::const_flat_iterator FlatTreeBegin(void) const;

        ModuleTree::const_flat_iterator FlatTreeEnd(void) const;


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
        std::map<std::string, std::unique_ptr<SupermoduleLoaderBase>> loadhandlers_;


        /*! \brief Actual storage object - maps module names to creation functions
         */
        std::map<std::string, StoreEntry> store_;


        /*! \brief Stores map of keys to module names
         */
        std::map<std::string, std::string> keymap_;


        /*! \brief List of keys with debugging info enabled */
        std::set<std::string> keydebug_;


        /*! \brief Debug for all? */
        bool debugall_;


        /*! \brief Tree for storing created module information
         */
        ModuleTree mtree_;


        //! The id to assign to the next created module
        std::atomic<ID_t> curid_;


        //! Modules currently in use
        std::set<ID_t> modules_inuse_;


        /*! \brief Map of cache data
         *
         * The key is a combination of the module name and version
         */
        std::map<std::string, datastore::CacheData> cachemap_;


        /*! \brief Obtain the module name for a key or throw an exception
         *
         * \throw bpmodule::exception::ModuleManagerException
         *        if the key doesn't exist
         */
        std::string GetOrThrowKey_(const std::string & modulekey) const;


        /*! \brief Obtain stored internal info for a module (via module name) or throw an exception
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


        /*! \brief Obtain stored internal info for a module (via module key) or throw an exception
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
        CreateModule_(const std::string & modulekey, ID_t parentid);

};



} // close namespace modulemanager
} // close namespace bpmodule


#endif
