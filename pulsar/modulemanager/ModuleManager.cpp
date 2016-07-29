/*! \file
 *
 * \brief Main module database class (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */
#include <random>
#include "pulsar/modulemanager/ModuleManager.hpp"
#include "pulsar/output/Output.hpp"
#include "pulsar/modulebase/ModuleBase.hpp"
#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/datastore/Wavefunction.hpp"
#include "pulsar/modulemanager/Checkpoint.hpp"
#include "pulsar/output/GlobalOutput.hpp"

////////////////////////
// Supermodule Handlers
////////////////////////
#include "pulsar/modulemanager/CppSupermoduleLoader.hpp"
#include "pulsar/modulemanager/PySupermoduleLoader.hpp"

using namespace pulsar::exception;
using namespace pulsar::datastore;
using namespace pulsar::modulebase;
using namespace pulsar::output;


namespace pulsar{
namespace modulemanager {


std::string ModuleManager::make_cache_key_(const ModuleInfo & mi)
{
    return mi.name + "_v" + mi.version;
}


ModuleManager::ModuleManager()
    : debugall_(false),
      curid_(100)
{
    // add the handlers
    loadhandlers_.emplace("c_module", std::unique_ptr<SupermoduleLoaderBase>(new CppSupermoduleLoader()));
    loadhandlers_.emplace("python_module", std::unique_ptr<SupermoduleLoaderBase>(new PySupermoduleLoader()));

    //! \todo pass in via ModuleAdministrator or something
    scratch_path_ = "/tmp";
}


ModuleManager::~ModuleManager()
{
    // wait for any checkpointing operations to finish
    if(checkpoint_thread_)
    {
        print_global_warning("Waiting for any checkpoint threads to finish\n");
        checkpoint_thread_->join();
        checkpoint_thread_.reset();
    }

    /*
     * Warn of any in-use modules
     */
    size_t ninuse = modules_inuse_.size();;
    if(ninuse)
        print_global_warning("ModuleManager is destructing with %? modules in use\n", ninuse);
    else
        print_global_success("ModuleManager has no modules in use\n");


    /*
       WARNING WARNING WARNING WARNING WARNING
       Clearing the cache and store MUST be done BEFORE unloading
       the modules or else deleting elements will cause a segfault.
       Reason: The GenericHolder is a template, so the code for
               the destructors exists in the modules
    */
    cachemap_.clear();
    store_.clear();

    /*
     * The tree contains System, so that must be cleared before unloading
     *  as well
     */
    mtree_.clear();



    // NOW we can close all the handlers
    loadhandlers_.clear();
}


size_t ModuleManager::size(void) const noexcept
{
    std::lock_guard<std::mutex> l(mutex_);
    return store_.size();
}


bool ModuleManager::has_key(const std::string & modulekey) const
{
    std::lock_guard<std::mutex> l(mutex_);
    return store_.count(modulekey);
}


ModuleInfo ModuleManager::module_key_info(const std::string & modulekey) const
{
    auto & se = get_or_throw_(modulekey);

    std::lock_guard<std::mutex> l(se.semutex);
    return se.mi;
}


void ModuleManager::duplicate_key(const std::string & modulekey, const std::string newkey)
{
    std::lock_guard<std::mutex> l(mutex_);
    if(store_.count(newkey))
        throw ModuleLoadException("Cannot duplicate key: new key already exists",
                                  "modulekey", modulekey, "newkey", newkey);

    // copy
    StoreEntry se = store_.at(modulekey);

    // reset the number of times this key has been called
    se.ncalled = 0;

    // move the copy to the new key
    store_.emplace(newkey, std::move(se));
}

std::string ModuleManager::generate_unique_key() const
{
    //! \todo still not guarenteed in multi-threaded contexts
    std::lock_guard<std::mutex> l(mutex_);
    std::random_device r;
    std::default_random_engine e(r());
    std::uniform_int_distribution<int> dist;
    std::stringstream ss;

    do {
        ss << dist(e);
    } while(store_.count(ss.str()));

    return ss.str();
}

void ModuleManager::print(std::ostream & os) const
{
    std::lock_guard<std::mutex> l(mutex_);
    for(const auto & it : store_)
        it.second.mi.print(os);
}


void ModuleManager::test_all(void)
{
    //! \todo needs rewriting and cleanup
    print_global_debug("Testing all modules\n");
    for(const auto & it : store_)
    {
        print_global_debug("Testing %? (%?)...\n", it.first, it.second.mi.name);

        if(!it.second.mi.options.all_req_set())
        {
            print_global_error("Error - module %? [key %?]\" failed options test - required options are missing\n", it.second.mi.name, it.first);

            auto missingreq = it.second.mi.options.all_missing_req();
            for(const auto & optit : missingreq)
                print_global_error("    Missing \"%?\"\n", optit);
        }


        print_global_debug("Test of %? OK\n", it.first);
    }

    print_global_debug("Testing getting of modules\n");
    for(const auto & it : store_)
    {
        try {
            get_module<ModuleBase>(it.first, 0);
        }
        catch(std::exception & ex)
        {
            print_global_error("Error - module %? [key %?]\" failed test loading!\n", it.second.mi.name, it.first);
            throw GeneralException(ex, "location", "test_all");
        }
    }

    print_global_debug("Testing done!\n");
}


ModuleManager::StoreEntry & ModuleManager::get_or_throw_(const std::string & modulekey)
{
    std::lock_guard<std::mutex> l(mutex_);
    if(store_.count(modulekey))
        return store_.at(modulekey);
    else
        throw ModuleManagerException("Missing module key in ModuleManager", "modulekey", modulekey);
}


const ModuleManager::StoreEntry & ModuleManager::get_or_throw_(const std::string & modulekey) const
{
    std::lock_guard<std::mutex> l(mutex_);
    if(store_.count(modulekey))
        return store_.at(modulekey);
    else
        throw ModuleManagerException("Missing module key in ModuleManager", "modulekey", modulekey);
}


void ModuleManager::enable_debug(const std::string & modulekey, bool debug)
{
    std::lock_guard<std::mutex> l(mutex_);
    if(debug)
        keydebug_.insert(modulekey);
    else
        keydebug_.erase(modulekey); // ok if it doesn't exist
}


void ModuleManager::enable_debug_all(bool debug) noexcept
{
    // mutex lock isn't needed - debugall is std::atomic<bool>
    debugall_ = debug;
}


/////////////////////////////////////////
// Module Loading
/////////////////////////////////////////
void ModuleManager::load_module_from_minfo(const ModuleInfo & minfo, const std::string & modulekey)
{
    //! \todo document as not thread safe

    // path set in the module info?
    if(minfo.path.size() == 0)
        throw ModuleLoadException("Cannot load module: Empty path",
                                  "modulepath", minfo.path, "modulename", minfo.name);

    // check for duplicates
    if(store_.count(modulekey))
        throw ModuleLoadException("Cannot load module: duplicate key",
                                  "modulepath", minfo.path, "modulename", minfo.name,
                                  "modulekey", modulekey);

    // Using the handler, load the module
    if(!loadhandlers_.count(minfo.type))
        throw ModuleLoadException("Cannot load module: unknown module type",
                                  "modulepath", minfo.path, "modulename", minfo.name, "type", minfo.type);

    // may throw an exception. Exception should be a ModuleLoadException
    const ModuleCreationFuncs & mcf = loadhandlers_.at(minfo.type)->load_supermodule(minfo.path);

    // See if this supermodule actually create a module with this name
    if(!mcf.has_creator(minfo.name))
        throw ModuleLoadException("Creators from this supermodule cannot create a module with this name",
                                  "path", minfo.path, "modulename", minfo.name);


    // add to store with the given module name
    // Arguments: module info, creator, ncalled
    StoreEntry se;
    se.mi = minfo;
    se.mc = mcf.get_creator(minfo.name);
    se.ncalled = 0;
    store_.emplace(modulekey, std::move(se));
}


void ModuleManager::notify_destruction(ID_t id)
{
    modules_inuse_.erase(id);
}


bool ModuleManager::module_in_use(ID_t id) const
{
    std::lock_guard<std::mutex> l(mutex_);
    return(modules_inuse_.count(id));
}



/////////////////////////////////////////
// Module Creation
/////////////////////////////////////////
std::unique_ptr<detail::ModuleIMPLHolder>
ModuleManager::create_module_(const std::string & modulekey, ID_t parentid)
{
    // obtain the information for this key
    // (will lock & release mutex)
    StoreEntry & se = get_or_throw_(modulekey);

    // lock everything for the duration of this function
    // it might be possible to split it, but we will see if this causes
    // problems (I'm expecting it won't)
    std::lock_guard<std::mutex> l(se.semutex);
    std::lock_guard<std::mutex> l2(mutex_);


    if(parentid != 0 && !mtree_.has_id(parentid))
        throw exception::ModuleCreateException("Parent does not exist on map", "parentid", parentid);


    // obtain the options and validate them.
    // Throw an exception if they are invalid (and not in expert mode)
    // NOTE - we do this first so that we don't create a module
    // if the options are bad (and an exception is thrown)
    OptionMapIssues iss = se.mi.options.get_issues();
    if(!iss.ok())
    {
        if(se.mi.options.is_expert())
        {
            print_global_warning("Options for module key %? has issues, but expert mode is on", modulekey);
            print_global_warning(iss.to_string());
        }
        else
        {
            std::stringstream ss;
            iss.print(ss);
            throw OptionException("Options for module are not valid", "modulekey", modulekey,
                                  "issues", ss.str());
        }
    }


    // add the moduleinfo to the tree
    // (parentid, etc, will be set by the tree)
    ModuleTreeNode me{modulekey,      // key
                      se.mi,          // module info
                      std::string(),  // output
                      curid_,         // module id
                     };

    // actually create the module
    std::unique_ptr<detail::ModuleIMPLHolder> umbptr;

    try {
      // calls the creation function within the loaded supermodule
      umbptr = std::unique_ptr<detail::ModuleIMPLHolder>(se.mc(curid_));
    }
    catch(const std::exception & ex)
    {
        throw exception::ModuleCreateException(ex,
                                               "path", se.mi.path,
                                               "modulekey", modulekey,
                                               "modulename", se.mi.name);
    }

    if(!umbptr)
        throw exception::ModuleCreateException("Module function returned a null pointer",
                                               "path", se.mi.path,
                                               "modulekey", modulekey,
                                               "modulename", se.mi.name);


    // move the data to the tree
    mtree_.insert(std::move(me), parentid);

    // "me" should not be accessed after this, so
    // we load a reference to it
    ModuleTreeNode & mtn = mtree_.get_by_id(curid_);


    // set the info for the module
    // (set via C++ functions)
    ModuleBase * p = umbptr->CppPtr();
    p->set_module_manager_(this);
    p->set_tree_node_(&mtn); // also sets up output tee


    // Debugging enabled for this module?
    if(debugall_ || keydebug_.count(modulekey))
        p->enable_debug(true);

    // Mark the id as in use
    modules_inuse_.emplace(curid_);

    // get this module's cache
    // don't use .at() -- we need it created if it doesn't exist already
    std::string mbstr = make_cache_key_(mtn.minfo);
    p->set_cache_(&(cachemap_[mbstr]));

    // next id
    curid_++;

    // increment the number of times this key has been
    // used in creation
    se.ncalled++;

    return std::move(umbptr);
}



////////////////////
// Python
////////////////////
pybind11::object ModuleManager::get_module_py(const std::string & modulekey,
                                            ID_t parentid)
{
    // mutex locking handled in create_module_
    std::unique_ptr<detail::ModuleIMPLHolder> umbptr = create_module_(modulekey, parentid);

    // we use a pointer so that the python object
    // can take ownership and we can avoid having
    // a copy constructor for PyModulePtr

    return python::convert_to_py(new PyModulePtr(std::move(umbptr)), pybind11::return_value_policy::take_ownership);
}

void ModuleManager::change_option_py(const std::string & modulekey, const std::string & optkey, const pybind11::object & value)
{
    StoreEntry & se = get_or_throw_(modulekey);

    std::lock_guard<std::mutex> l(se.semutex);
    if(se.ncalled != 0)
        throw ModuleManagerException("Attempting to change options for a previously-used module key", "modulekey", modulekey, "optkey", optkey);

    try {
        se.mi.options.change_py(optkey, value);
    }
    catch(exception::GeneralException & ex)
    {
        ex.append_info("modulekey", modulekey);
        throw;
    }
}


////////////////////
// Checkpointing
////////////////////

static void checkpoint_thread_function_(const ModuleManager & mm, std::string path)
{
    Checkpoint cp(path);
    cp.save(mm); 
}

void ModuleManager::run_checkpoint(bool separate_thread) const
{
    // wait for any existing checkpointing operations to finish
    if(checkpoint_thread_)
    {
        print_global_warning("Waiting for existing checkpoint thread to finish\n");
        checkpoint_thread_->join();
        checkpoint_thread_.reset();
    }

    // always create another thread (because I'm lazy)
    std::thread th(checkpoint_thread_function_, std::ref(*this), scratch_path_);
    checkpoint_thread_ = std::unique_ptr<std::thread>(new std::thread(std::move(th)));

    // if we don't want a separate thread, just join and wait for the
    // new thread to finish
    if(!separate_thread)
    {
        checkpoint_thread_->join();
        checkpoint_thread_.reset();
    }
}



} // close namespace modulemanager
} // close namespace pulsar
