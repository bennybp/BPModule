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


static std::string MakeCacheKey(const ModuleInfo & mi)
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
}


ModuleManager::~ModuleManager()
{
    /*
     * Warn of any in-use modules
     */
    size_t ninuse = 0;
    for(auto it = mtree_.FlatBegin(); it != mtree_.FlatEnd(); ++it)
        ninuse += (it->inuse ? 1 : 0);
                        
    if(ninuse)
        GlobalWarning("ModuleManager is destructing with %? modules in use\n", ninuse);
    else
        GlobalSuccess("ModuleManager has no modules in use\n");


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
    mtree_.Clear();



    // NOW we can close all the handlers
    loadhandlers_.clear();
}


size_t ModuleManager::Size(void) const noexcept
{
    return store_.size();
}


bool ModuleManager::HasKey(const std::string & modulekey) const
{
    return store_.count(modulekey);
}


ModuleInfo ModuleManager::ModuleKeyInfo(const std::string & modulekey) const
{
    return GetOrThrow_(modulekey).mi;
}


void ModuleManager::DuplicateKey(const std::string & modulekey, const std::string newkey)
{
    if(HasKey(newkey))
        throw ModuleLoadException("Cannot duplicate key: new key already exists",
                                  "modulekey", modulekey, "newkey", newkey);

    // copy
    StoreEntry se = GetOrThrow_(modulekey);

    // set the number of times this key has been called
    se.ncalled = 0;

    store_.emplace(newkey, std::move(se));
}

std::string ModuleManager::GenerateUniqueKey() const{
    std::random_device r;
    std::default_random_engine e(r());
    std::uniform_int_distribution<int> dist;
    std::stringstream ss;
    do{
        ss<<dist(e);
    }while(HasKey(ss.str()));
    return ss.str();
}

void ModuleManager::Print(std::ostream & os) const
{
    for(const auto & it : store_)
        it.second.mi.Print(os);
}


void ModuleManager::TestAll(void)
{
    //! \todo needs rewriting and cleanup
    GlobalDebug("Testing all modules\n");
    for(const auto & it : store_)
    {
        GlobalDebug("Testing %? (%?)...\n", it.first, it.second.mi.name);

        if(!it.second.mi.options.AllReqSet())
        {
            GlobalError("Error - module %? [key %?]\" failed options test - required options are missing\n", it.second.mi.name, it.first);

            auto missingreq = it.second.mi.options.AllMissingReq();
            for(const auto & optit : missingreq)
                GlobalError("    Missing \"%?\"\n", optit);
        }


        GlobalDebug("Test of %? OK\n", it.first);
    }

    GlobalDebug("Testing getting of modules\n");
    for(const auto & it : store_)
    {
        try {
            GetModule<ModuleBase>(it.first, 0);
        }
        catch(std::exception & ex)
        {
            GlobalError("Error - module %? [key %?]\" failed test loading!\n", it.second.mi.name, it.first);
            throw GeneralException(ex, "location", "TestAll");
        }
    }

    GlobalDebug("Testing done!\n");
}


ModuleManager::StoreEntry & ModuleManager::GetOrThrow_(const std::string & modulekey)
{
    if(HasKey(modulekey))
        return store_.at(modulekey);
    else
        throw ModuleManagerException("Missing module key in ModuleManager", "modulekey", modulekey);
}


const ModuleManager::StoreEntry & ModuleManager::GetOrThrow_(const std::string & modulekey) const
{
    if(HasKey(modulekey))
        return store_.at(modulekey);
    else
        throw ModuleManagerException("Missing module key in ModuleManager", "modulekey", modulekey);
}


void ModuleManager::EnableDebug(const std::string & modulekey, bool debug)
{
    if(debug)
        keydebug_.insert(modulekey);
    else
        keydebug_.erase(modulekey); // ok if it doesn't exist
}


void ModuleManager::EnableDebugAll(bool debug) noexcept
{
    debugall_ = debug;
}


ModuleTree::const_iterator ModuleManager::TreeBegin(ID_t startid) const
{
    return mtree_.Begin(startid);
}

ModuleTree::const_iterator ModuleManager::TreeEnd(void) const
{
    return mtree_.End();
}

ModuleTree::const_flat_iterator ModuleManager::FlatTreeBegin(void) const
{
    return mtree_.FlatBegin();
}

ModuleTree::const_flat_iterator ModuleManager::FlatTreeEnd(void) const
{
    return mtree_.FlatEnd();
}


/////////////////////////////////////////
// Module Loading
/////////////////////////////////////////
void ModuleManager::LoadModuleFromModuleInfo(const ModuleInfo & minfo, const std::string & modulekey)
{
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
    const ModuleCreationFuncs & mcf = loadhandlers_.at(minfo.type)->LoadSupermodule(minfo.path);

    // See if this supermodule actually create a module with this name
    if(!mcf.HasCreator(minfo.name))
        throw ModuleLoadException("Creators from this supermodule cannot create a module with this name",
                                  "path", minfo.path, "modulename", minfo.name);

    // add to store with the given module name
    // Arguments: module info, creator, ncalled
    store_.emplace(modulekey, StoreEntry{minfo, mcf.GetCreator(minfo.name), 0});
}


bool ModuleManager::ModuleInUse(ID_t id) const
{
    if(!mtree_.HasID(id))
        return false;
    else
        return mtree_.GetByID(id).inuse;
}


/////////////////////////////////////////
// Module Creation
/////////////////////////////////////////
std::unique_ptr<detail::ModuleIMPLHolder>
ModuleManager::CreateModule_(const std::string & modulekey, ID_t parentid)
{
    // obtain the information for this key
    StoreEntry & se = GetOrThrow_(modulekey);

    // obtain the options and validate them.
    // Throw an exception if they are invalid (and not in expert mode)
    // NOTE - we do this first so that we don't create a module
    // if the options are bad (and an exception is thrown)
    ModuleInfo mi(se.mi);

    OptionMapIssues iss = se.mi.options.GetIssues();
    if(!iss.OK())
    {
        if(se.mi.options.IsExpert())
        {
            GlobalWarning("Options for module key %? has issues, but expert mode is on", modulekey);
            GlobalWarning(iss.String());
        }
        else
        {
            std::stringstream ss;
            iss.Print(ss);
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
                      Wavefunction(), // initial wfn
                      Wavefunction(), // final wfn
                      true            // module is in use
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


    // If there is a parent, get its wavefunction and use that
    if(parentid != 0)
    {
        if(!mtree_.HasID(parentid))
            throw exception::ModuleCreateException("Parent does not exit on map", "parentid", parentid);

        const ModuleTreeNode & parent = mtree_.GetByID(parentid);

        // if the parent is inuse, use its initial wfn. Else, use its final
        if(ModuleInUse(parentid))
        {
            GlobalDebug("Module %? - using initial wavefunction of inuse parent %?\n", static_cast<ID_t>(curid_), parentid);
            me.initial_wfn = parent.initial_wfn;
            me.final_wfn = parent.initial_wfn;
        }
        else
        {
            GlobalDebug("Module %? - using final wavefunction of destroyed parent %?\n", static_cast<ID_t>(curid_), parentid);
            me.initial_wfn = parent.final_wfn;
            me.final_wfn = parent.final_wfn;
        }
    }

    // move the data to the tree
    // "me" should not be accessed after this, so
    // we load a reference to it
    mtree_.Insert(std::move(me), parentid);
    ModuleTreeNode & mtn = mtree_.GetByID(curid_);


    // set the info for the module
    // (set via C++ functions)
    ModuleBase * p = umbptr->CppPtr();
    p->SetMManager_(this);
    p->SetTreeNode_(&mtn); // also sets up output tee


    // Debugging enabled for this module?
    if(debugall_ || keydebug_.count(modulekey))
        p->EnableDebug(true);


    // get this module's cache
    // don't use .at() -- we need it created if it doesn't exist already
    std::string mbstr = MakeCacheKey(mtn.minfo);
    p->SetCache_(&(cachemap_[mbstr]));

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
pybind11::object ModuleManager::GetModulePy(const std::string & modulekey,
                                            ID_t parentid)
{
    std::unique_ptr<detail::ModuleIMPLHolder> umbptr = CreateModule_(modulekey, parentid);

    // we use a pointer so that the python object
    // can take ownership and we can avoid having
    // a copy constructor for PyModulePtr

    return python::ConvertToPy(new PyModulePtr(std::move(umbptr)), pybind11::return_value_policy::take_ownership);
}

void ModuleManager::ChangeOptionPy(const std::string & modulekey, const std::string & optkey, const pybind11::object & value)
{
    StoreEntry & se = GetOrThrow_(modulekey);
    if(se.ncalled != 0)
        throw ModuleManagerException("Attempting to change options for a previously-used module key", "modulekey", modulekey, "optkey", optkey);

    try {
        se.mi.options.ChangePy(optkey, value);
    }
    catch(exception::GeneralException & ex)
    {
        ex.AppendInfo("modulekey", modulekey);
        throw;
    }
}


} // close namespace modulemanager
} // close namespace pulsar
