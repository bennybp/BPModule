/*! \file
 *
 * \brief Main module database class (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "bpmodule/modulemanager/ModuleManager.hpp"
#include "bpmodule/output/Output.hpp"
#include "bpmodule/modulebase/ModuleBase.hpp"
#include "bpmodule/exception/Exceptions.hpp"
#include "bpmodule/datastore/Wavefunction.hpp"
#include "bpmodule/output/GlobalOutput.hpp"

////////////////////////
// Supermodule Handlers
////////////////////////
#include "bpmodule/modulemanager/CppSupermoduleLoader.hpp"
#include "bpmodule/modulemanager/PySupermoduleLoader.hpp"


using bpmodule::datastore::Wavefunction;
using bpmodule::modulebase::ModuleBase;
using bpmodule::exception::ModuleLoadException;
using bpmodule::exception::ModuleManagerException;
using bpmodule::exception::GeneralException;


namespace bpmodule {
namespace modulemanager {



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
     * Delete any in-use modules
     */
    auto ninuse = modules_inuse_.size();
    if(ninuse)
        output::GlobalWarning("ModuleManager is destructing with %? modules in use\n", ninuse);
    else
        output::GlobalSuccess("ModuleManager has no modules in use\n");


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

void ModuleManager::Print(std::ostream & os) const
{
    for(const auto & it : store_)
        it.second.mi.Print(os);
}


void ModuleManager::TestAll(void)
{
    //! \todo needs rewriting and cleanup
    output::GlobalDebug("Testing all modules\n");
    for(const auto & it : store_)
    {
        output::GlobalDebug("Testing %? (%?)...\n", it.first, it.second.mi.name);

        if(!it.second.mi.options.AllReqSet())
        {
            output::GlobalError("Error - module %? [key %?]\" failed options test - required options are missing", it.second.mi.name, it.first);

            auto missingreq = it.second.mi.options.AllMissingReq();
            for(const auto & optit : missingreq)
                output::GlobalError("    Missing \"%?\"\n", optit);
        }


        output::GlobalDebug("Test of %? OK\n", it.first);
    }

    output::GlobalDebug("Testing getting of modules\n");
    for(const auto & it : store_)
    {
        try {
            GetModule<ModuleBase>(it.first, 0);
        }
        catch(std::exception & ex)
        {
            output::GlobalError("Error - module %? [key %?]\" failed test loading!\n", it.second.mi.name, it.first);
            throw GeneralException(ex, "location", "TestAll");
        }
    }

    output::GlobalDebug("Testing done!\n");
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
    if(store_.count(minfo.name))
        throw ModuleLoadException("Cannot load module: duplicate module name",
                                  "modulepath", minfo.path, "modulename", minfo.name);

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
    return modules_inuse_.count(id);
}

void ModuleManager::DestructionNotify(ID_t id) noexcept
{
   // comparison object doesn't throw, therefore this shouldn't throw
   modules_inuse_.erase(id);
}


/////////////////////////////////////////
// Module Creation
/////////////////////////////////////////
std::unique_ptr<detail::ModuleIMPLHolder>
ModuleManager::CreateModule_(const std::string & modulekey, ID_t parentid)
{
    // obtain the information for this key
    StoreEntry & se = GetOrThrow_(modulekey);

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


    // add the moduleinfo to the tree
    ModuleTreeNode me{modulekey,      // key
                      se.mi,          // module info
                      std::string(),  // output
                      curid_,         // module id
                      Wavefunction(), // initial wfn
                      Wavefunction()  // final wfn
                     };


    // If there is a parent, get its wavefunction and use that
    if(parentid != 0)
    {
        if(!mtree_.HasID(parentid))
            throw exception::ModuleCreateException("Parent does not exit on map", "parentid", parentid);

        const ModuleTreeNode & parent = mtree_.GetByID(parentid);
        me.initial_wfn = parent.initial_wfn;
        me.final_wfn = parent.initial_wfn;
    }

    // move the data to the tree
    // "me" should not be accessed after this
    mtree_.Insert(std::move(me), parentid);


    // set the info for the module
    // (set via C++ functions)
    ModuleBase * p = umbptr->CppPtr();
    p->SetMManager_(this);
    p->SetTreeNode_(&(mtree_.GetByID(curid_))); // also sets up output tee


    // Debugging enabled for this module?
    if(debugall_ || keydebug_.count(modulekey))
        p->EnableDebug(true);


    // get this module's cache
    // don't use .at() -- we need it created if it doesn't exist already
    std::string mbstr = p->Name() + "_v" + p->Version();
    p->SetCache_(&(cachemap_[mbstr]));

    // mark the module as inuse
    modules_inuse_.insert(curid_);

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

    se.mi.options.ChangePy(optkey, value);
}


} // close namespace modulemanager
} // close namespace bpmodule
