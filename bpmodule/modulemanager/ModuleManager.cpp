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

////////////////////////
// Supermodule Handlers
////////////////////////
#include "bpmodule/modulemanager/CppSupermoduleLoader.hpp"
#include "bpmodule/modulemanager/PySupermoduleLoader.hpp"


using bpmodule::datastore::ModuleGraphNodeData;
using bpmodule::datastore::ModuleGraphNode;
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
       WARNING WARNING WARNING WARNING WARNING
       Clearing the cache and store MUST be done BEFORE unloading
       the modules or else deleting elements will cause a segfault.
       Reason: The GenericHolder is a template, so the code for
               the destructors exists in the modules
    */
    cachemap_.clear();
    keymap_.clear();
    store_.clear();

    // NOW we can close all the handlers
    loadhandlers_.clear();
}


void ModuleManager::AddKey(const std::string & modulekey, const std::string & modulename)
{
    if(!HasName(modulename))
        throw ModuleManagerException("Cannot add key: The given module name doesn't exist",
                                     "modulekey", modulekey, "modulename", modulename);
    if(HasKey(modulekey))
        throw ModuleManagerException("Cannot add key: Key already exists",
                                     "modulekey", modulekey, "modulename", modulename);

    keymap_.emplace(modulekey, modulename);
}


void ModuleManager::ReplaceKey(const std::string & modulekey, const std::string & modulename)
{
    if(!HasName(modulename))
        throw ModuleManagerException("Cannot add key: The given module name doesn't exist",
                                     "modulekey", modulekey, "modulename", modulename);

    keymap_.erase(modulekey); // ok if the key doesn't exist
    AddKey(modulekey, modulename);
}


size_t ModuleManager::Size(void) const noexcept
{
    return store_.size();
}


bool ModuleManager::HasKey(const std::string & modulekey) const
{
    return keymap_.count(modulekey);
}


bool ModuleManager::HasName(const std::string & modulename) const
{
    return store_.count(modulename);
}


ModuleInfo ModuleManager::ModuleKeyInfo(const std::string & modulekey) const
{
    return GetOrThrow_(modulekey).mi;
}


ModuleInfo ModuleManager::ModuleNameInfo(const std::string & modulename) const
{
    return GetOrThrowName_(modulename).mi;
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
    for(const auto & it : keymap_)
    {
        try {
            GetModule<ModuleBase>(it.first, 0);
        }
        catch(std::exception & ex)
        {
            output::GlobalError("Error - module %? [key %?]\" failed test loading!\n", it.second, it.first);
            throw GeneralException(ex, "location", "TestAll");
        }
    }


    output::GlobalDebug("Testing done!\n");
}


std::string ModuleManager::GetOrThrowKey_(const std::string & modulekey) const
{
    if(HasKey(modulekey))
        return keymap_.at(modulekey);
    else
        throw ModuleManagerException("Missing module key in ModuleManager", "modulekey", modulekey);
}


ModuleManager::StoreEntry & ModuleManager::GetOrThrowName_(const std::string & modulename)
{
    if(HasName(modulename))
        return store_.at(modulename);
    else
        throw ModuleManagerException("Missing module name in ModuleManager", "modulename", modulename);
}


const ModuleManager::StoreEntry & ModuleManager::GetOrThrowName_(const std::string & modulename) const
{
    if(HasName(modulename))
        return store_.at(modulename);
    else
        throw ModuleManagerException("Missing module name in ModuleManager", "modulename", modulename);
}


ModuleManager::StoreEntry & ModuleManager::GetOrThrow_(const std::string & modulekey)
{
    return GetOrThrowName_(GetOrThrowKey_(modulekey));
}


const ModuleManager::StoreEntry & ModuleManager::GetOrThrow_(const std::string & modulekey) const
{
    return GetOrThrowName_(GetOrThrowKey_(modulekey));
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



/////////////////////////////////////////
// Module Loading
/////////////////////////////////////////
void ModuleManager::LoadModuleFromModuleInfo(const ModuleInfo & minfo)
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
    store_.emplace(minfo.name, StoreEntry{minfo, mcf.GetCreator(minfo.name)});
}





/////////////////////////////////////////
// Module Creation
/////////////////////////////////////////
std::unique_ptr<detail::ModuleIMPLHolder>
ModuleManager::CreateModule_(const std::string & modulekey, unsigned long parentid)
{
    // obtain the information for this key
    const StoreEntry & se = GetOrThrow_(modulekey);

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

 
    // add the moduleinfo to the graph
    //! \todo Assumes shared pointer, requires map, etc
    if(parentid != 0)
    {
        ModuleGraphNode parent = mgraphmap_.at(parentid);
        ModuleGraphNode mine{new ModuleGraphNodeData{modulekey, parent->wfn, se.mi}};
        mgraph_.AddNode(mine);
        mgraph_.AddEdge(std::make_tuple(mine, parent));
        mgraphmap_.emplace(curid_, mine);
    }
    else
    {
        ModuleGraphNode mine{new ModuleGraphNodeData{modulekey, Wavefunction(), se.mi}};
        mgraph_.AddNode(mine);
        mgraphmap_.emplace(curid_, mine);
    }


    // set the info for the module
    // (set via C++ functions)
    ModuleBase * p = umbptr->CppPtr();
    p->SetMManager_(this);
    p->SetGraphNode_(&(mgraphmap_.at(curid_))); // also sets up output tee

    // Debugging?
    if(debugall_ || keydebug_.count(modulekey))
        p->EnableDebug(true);

    // get this module's cache
    // no need to use .at() -- we need it created if it doesn't exist already
    std::string mbstr = p->Name() + "_v" + p->Version();
    p->SetCache_(&(cachemap_[mbstr]));

    // next id
    curid_++;

    return std::move(umbptr);
}


std::string ModuleManager::DotGraph(void) const
{
    std::stringstream ss;
    ss << mgraph_;
    return ss.str();
}


////////////////////
// Python
////////////////////
pybind11::object ModuleManager::GetModulePy(const std::string & modulekey,
                                            unsigned long parentid)
{
    std::unique_ptr<detail::ModuleIMPLHolder> umbptr = CreateModule_(modulekey, parentid);

    // we use a pointer so that the python object
    // can take ownership and we can avoid having
    // a copy constructor for PyModulePtr
    return python::ConvertToPy(new PyModulePtr(std::move(umbptr)), pybind11::return_value_policy::take_ownership);
}
        
void ModuleManager::ChangeOptionPy(const std::string & modulekey, const std::string & optkey, const pybind11::object & value)
{
    GetOrThrow_(modulekey).mi.options.ChangePy(optkey, value);
}


} // close namespace modulemanager
} // close namespace bpmodule
