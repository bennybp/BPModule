/*! \file
 *
 * \brief Main module database class (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_MODULEMANAGER__MODULEMANAGER_HPP_
#define PULSAR_GUARD_MODULEMANAGER__MODULEMANAGER_HPP_

#include <atomic>
#include <thread>

#include "pulsar/datastore/CacheData.hpp"
#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/modulemanager/ModuleCreationFuncs.hpp"
#include "pulsar/modulemanager/ModuleTree.hpp"
#include "pulsar/modulemanager/ModulePtr.hpp"



namespace pulsar{
namespace modulemanager {


// forward declaration
class SupermoduleLoaderBase;
class Checkpoint;


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
        ModuleManager(ModuleManager && rhs)                  = default;
        ModuleManager & operator=(const ModuleManager & rhs) = delete;
        ModuleManager & operator=(ModuleManager && rhs)      = default;


        /*! \brief Returns the number of modules in the database
         *
         * \exnothrow
         */
        size_t size(void) const noexcept;


        /*! \brief Returns the information about a module with a given module key
         *
         * \throw pulsar::exception::ModuleManagerException
         *        if the module key or its associated name doesn't exist in the database
         *
         * \param [in] modulekey A module key
         */
        ModuleInfo module_key_info(const std::string & modulekey) const;


        /*! \brief prints all the information about the loaded modules
         */
        void print(std::ostream & os) const;


        /*! \brief Returns true if a module with the given module key exists in the database
         *
         * \param [in] modulekey A module key
         * \return True if the key exists in the map, false if it doesn't
         */
        bool has_key(const std::string & modulekey) const;


        /*! \brief Test the creation of all modules
         *
         * This goes through and tests the module creation for all keys. It does
         * not attempt to cast them, though. This is a simple sanity check
         *
         * \throw pulsar::exception::ModuleCreateException if there is a problem
         *
         * \exbasic
         * \todo make strong?
         * \todo not threadsafe - shouldn't be
         */
        void test_all(void);


        /*! \brief Create a new module as a wrapped C++ object
         *
         * The wavefunction that is set for the initial and final
         * wavefunctions of the new module depend on whether or
         * not the parent module is in use. If it is in use, the wavefunctions
         * are set to the initial wavefunction of the parent, since it is assumed
         * that it has not completed yet.
         *
         * If the parent is not in use, the wavefunction is set to the final
         * wavefunction of the parent.
         *
         * \throw pulsar::exception::ModuleManagerException
         *        if the key doesn't exist in the database
         *
         * \throw pulsar::exception::ModuleCreateException if there are other
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
        ModulePtr<T> get_module(const std::string & modulekey, ID_t parentid)
        {
            // mutex locking handled create_module_
            // may throw
            std::unique_ptr<detail::ModuleIMPLHolder> umbptr = create_module_(modulekey, parentid);

            if(!umbptr->IsType<T>())
                throw exception::ModuleCreateException("Module for this key is not of the right type",
                                                       "modulekey", modulekey,
                                                       "expectedtype", util::demangle_cpp_type<T>());

            // create the ModulePtr type
            ModulePtr<T> mod(std::move(umbptr));

            return mod;
        }


        /*! \brief Retrieve a module as a python object
         *
         * The wavefunction that is set for the initial and final
         * wavefunctions of the new module depend on whether or
         * not the parent module is in use. If it is in use, the wavefunctions
         * are set to the initial wavefunction of the parent, since it is assumed
         * that it has not completed yet.
         *
         * If the parent is not in use, the wavefunction is set to the final
         * wavefunction of the parent.
         *
         * \throw pulsar::exception::ModuleManagerException
         *        if the key doesn't exist in the database
         *
         * \throw pulsar::exception::ModuleCreateException if there are other
         *        problems creating the module
         *
         * \exbasic
         * \todo make strong?
         *
         * \return The module wrapped in a python object
         */
        pybind11::object get_module_py(const std::string & modulekey, ID_t parentid);

        /*! \brief Check to see if a module is currently in use
         */
        bool module_in_use(ID_t id) const;


        /*! \brief Change an option for a module
         * 
         * \throw pulsar::exception::ModuleManagerException
         *        if the key doesn't exist in the database or
         *        if options can't be changed since the module
         *        has been used.
         *
         * \param [in] modulekey The key of the module whose options to change
         * \param [in] optkey The key of the option to change
         * \param [in] value The value to set the option to
         */
        template<typename T>
        void change_option(const std::string & modulekey, const std::string & optkey, const T & value)
        {
            using namespace exception;

            StoreEntry & se = get_or_throw_(modulekey);

            std::lock_guard<std::mutex> l(se.semutex);

            if(se.ncalled != 0)
                throw ModuleManagerException("Attempting to change options for a previously-used module key",
                                             "modulekey", modulekey, "optkey", optkey);
            try {
                se.mi.options.change(optkey, value);
            }
            catch(exception::GeneralException & ex)
            {
                ex.append_info("modulekey", modulekey);
                throw;
            }
        }


        /*! \brief Duplicate information about a key to a new key
         *
         * Options, etc, are copied from the original key.
         *
         * \throw pulsar::exception::ModuleManagerException
         *        if the key doesn't exist in the database
         *
         * \param [in] modulekey Existing module key to duplicate
         * \param [in] newkey New key to be associated with the module
         */
        void duplicate_key(const std::string & modulekey, const std::string newkey);

        /*! \brief Generates a unique module key
         *
         *   Internally within a module there arises situations in which the
         *   module needs to duplicate a module so that it can tweak some
         *   options.  The resulting module must have a unique key.  This
         *   function generates a random unique key that your module can
         *   use in these circumstances.
         * 
         *   \return A random unique key that is not tied to any module yet
         */
        std::string generate_unique_key(void)const;
        

        /*! \brief Change an option for a module (python version)
         * 
         * \throw pulsar::exception::ModuleManagerException
         *        if the key doesn't exist in the database or
         *        if options can't be changed since the module
         *        has been used.
         *
         * \param [in] modulekey The key of the module whose options to change
         * \param [in] optkey The key of the option to change
         * \param [in] value The value to set the option to
         */
        void change_option_py(const std::string & modulekey, const std::string & optkey, const pybind11::object & value);



        /*! \brief Adds/inserts a module creator to the database
         *
         * The supermodule is loaded via a handler, and then info
         * for the module is extracted from the supermodule information.
         *
         * \throw pulsar::exception::ModuleLoaderException if the key
         *        already exists in the database or if \p mc doesn't
         *        contain a creator for the given module name (in \p mi)
         *
         *  \note We pass all module creation funcs. This is so we
         *        don't need to export IMPL holders to pybind11
         *
         * \param [in] minfo Information about the module
         * \param [in] modulekey The key to associate with the module
         */
        void load_module_from_minfo(const ModuleInfo & minfo, const std::string & modulekey);


        /*! \brief Enable debugging for a specific key
         *
         * The key doesn't have to exist -- it will be used if it is ever loaded
         */
        void enable_debug(const std::string & modulekey, bool debug);


        /*! \brief Enable debug printing on all modules
         */
        void enable_debug_all(bool debug) noexcept;

        /*! \brief Notify the module manager that a module ID is no longer in use */
        void notify_destruction(ID_t id);

    private:
        friend class Checkpoint;

        //! Protects the class in multi-thread operations
        mutable std::mutex mutex_;

        //! Threads currently running checkpointing operations
        mutable std::unique_ptr<std::thread> checkpoint_thread_;

        //! My scratch path (for checkpointing, etc)
        std::string scratch_path_;

        /*! \brief An entry for a module in the database
         */
        struct StoreEntry
        {
            ModuleInfo mi;                //!< Information for this module
            ModuleCreationFuncs::Func mc; //!< Function to create this module
            unsigned int ncalled;         //!< Number of times this key has been used for creation
            mutable std::mutex semutex;   //!< Mutex for this module's information

            StoreEntry() = default;
            StoreEntry(StoreEntry &&) = default;

            // we need a custom copy constructore due to the std::mutex
            StoreEntry(const StoreEntry & rhs)
                : mi(rhs.mi), mc(rhs.mc), ncalled(rhs.ncalled)
            { }
        };


        /*! \brief Handlers for different module types
         */
        std::map<std::string, std::unique_ptr<SupermoduleLoaderBase>> loadhandlers_;


        /*! \brief Actual storage object - maps module keys to creation functions
         */
        std::map<std::string, StoreEntry> store_;


        /*! \brief List of keys with debugging info enabled */
        std::set<std::string> keydebug_;


        /*! \brief Debug for all? */
        std::atomic<bool> debugall_;


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


        /*! \brief Obtain stored internal info for a module (via module key) or throw an exception
         *
         * \throw pulsar::exception::ModuleManagerException
         *        if the key or name doesn't exist
         *
         * \param [in] modulekey A module key
         */
        const StoreEntry & get_or_throw_(const std::string & modulekey) const;


        /*! \copydoc get_or_throw_
         */
        StoreEntry & get_or_throw_(const std::string & modulekey);



        /*! \brief Create a module via its creator function
         *
         * \throw pulsar::exception::ModuleManagerException
         *        if the key doesn't exist in the database
         *
         * \throw pulsar::exception::ModuleCreateException if there are other
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
        create_module_(const std::string & modulekey, ID_t parentid);


        /*! \brief Create a key for the cache given a ModuleInfo */
        static std::string make_cache_key_(const ModuleInfo & mi);
};

/** \brief Given an existing module, the ID of the new module's parent, a module manager, and a map (option_key to value) of options to change.  This function will copy the module into a unique key, change the options, and return the module implementation.
 
 TODO: Make a C++ version
 
 */
pybind11::object copy_key_change_options_py(const std::string& modulekey, 
                                            ID_t parentid,ModuleManager& mm,
    const std::map<std::string,pybind11::object>& options);



} // close namespace modulemanager
} // close namespace pulsar


#endif
