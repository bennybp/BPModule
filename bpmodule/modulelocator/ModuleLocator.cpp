/*! \file
 *
 * \brief Main module database class (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include <boost/python/dict.hpp>

#include "bpmodule/modulelocator/ModuleLocator.hpp"
#include "bpmodule/output/Output.hpp"
#include "bpmodule/modulebase/ModuleBase.hpp"
#include "bpmodule/exception/ModuleLoadException.hpp"

using bpmodule::modulebase::ModuleBase;
using bpmodule::exception::ModuleLoadException;
using bpmodule::exception::ModuleLocatorException;
using bpmodule::exception::GeneralException;


namespace bpmodule {
namespace modulelocator {



ModuleLocator::ModuleLocator()
    : curid_(0)
{ }



void ModuleLocator::InsertModule(const std::string & key, ModuleGeneratorFunc func,
                               ModuleRemoverFunc dfunc, const ModuleInfo & minfo)
{
    // add to store
    // but throw if key already exists
    if(store_.count(key))
        throw ModuleLoadException("Cannot insert module: duplicate key",
                                  minfo.path, key, minfo.name);

    store_.emplace(key, StoreEntry{minfo, func, dfunc});
}


void ModuleLocator::SetOptions(const std::string & key, const boost::python::dict & opt)
{
    GetOrThrow_(key).mi.options.ChangePyDict(opt);
}


size_t ModuleLocator::Size(void) const noexcept
{
    return store_.size();
}


std::vector<std::string> ModuleLocator::GetKeys(void) const
{
    std::vector<std::string> vec;
    vec.reserve(store_.size());
    for(const auto & it : store_)
        vec.push_back(it.first);
    return vec;
}



bool ModuleLocator::Has(const std::string & key) const
{
    return store_.count(key);
}



ModuleInfo ModuleLocator::KeyInfo(const std::string & key) const
{
    return GetOrThrow_(key).mi;
}



void ModuleLocator::PrintInfo(void) const
{
    for(const auto & it : store_)
        it.second.mi.Print();
}


void ModuleLocator::TestAll(void)
{
    output::Debug("Testing all modules\n");
    for(const auto & it : store_)
    {
        output::Debug("Testing %1% (%2%)...\n", it.first, it.second.mi.name);

        if(!it.second.mi.options.AllReqSet())
        {
            output::Error("Error - module %1% [key %2%]\" failed options test - required options are missing", it.second.mi.name, it.first);

            auto missingreq = it.second.mi.options.AllMissingReq();
            for(const auto & optit : missingreq)
                output::Error("    Missing \"%1%\"\n", optit);
        }

        try {
            GetModule<ModuleBase>(it.first);
        }
        catch(GeneralException & ex)
        {
            output::Error("Error - module %1% [key %2%]\" failed test loading!\n", it.second.mi.name, it.first);
            ex.AppendInfo("location", "TestAll");
            throw;
        }

        output::Debug("Test of %1% OK\n", it.first);
    }
}


void ModuleLocator::DeleteObject_(ModuleBase * mb)
{
    DeleteObject_(mb->ID());
}

void ModuleLocator::DeleteObject_(unsigned long id)
{
    if(removemap_.count(id))
    {
        ModuleRemoverFunc dfunc = removemap_.at(id);

        // Throwing from destructor is bad
        bool destructorthrew = false;

        try {
            dfunc(id);
        }
        catch(...)
        {
            destructorthrew = true;
        }

        removemap_.erase(id);
        //graphdata_.erase(id);

        if(destructorthrew)
            throw exception::ModuleLocatorException("Destructor for a module threw an exception",
                                                    "moduleid", id,
                                                    "name", graphdata_.at(id).minfo.name,
                                                    "key", graphdata_.at(id).minfo.key);

    }
}



const ModuleLocator::StoreEntry & ModuleLocator::GetOrThrow_(const std::string & key) const
{
    if(Has(key))
        return store_.at(key);
    else
        throw ModuleLocatorException("Missing module key",
                                   "location", "ModuleLocator",
                                   "modulekey", key);
}


ModuleLocator::StoreEntry & ModuleLocator::GetOrThrow_(const std::string & key)
{
    if(Has(key))
        return store_.at(key);
    else
        throw ModuleLocatorException("Missing module key",
                                   "location", "ModuleLocator",
                                   "modulekey", key);
}


std::pair<ModuleBase *, std::function<void(modulebase::ModuleBase *)>>
ModuleLocator::CreateModule(const std::string & key)
{
    // obtain the creator
    const StoreEntry & se = GetOrThrow_(key);

    // create
    modulebase::ModuleBase * mbptr = nullptr;
    try {
      mbptr = se.func(se.mi.name, curid_);
    }
    catch(const exception::GeneralException & gex)
    {
        throw exception::ModuleCreateException(gex,
                                               se.mi.path,
                                               se.mi.key,
                                               se.mi.name);
    }

    if(mbptr == nullptr)
        throw exception::ModuleCreateException("Create function returned a null pointer",
                                               se.mi.path,
                                               se.mi.key,
                                               se.mi.name);
    
    // add the moduleinfo to the graph
    // \todo Molecule, basis set, inherited from parent
    GraphNodeData gdata{nullptr, nullptr, se.mi, datastore::CalcData()};
    graphdata_.emplace(curid_, gdata);

    // set the info
    mbptr->SetMLocator_(this);
    mbptr->SetGraphData_(&(graphdata_.at(curid_)));

    // make the deleter function the DeleteObject_() of this ModuleLocator object
    std::function<void(modulebase::ModuleBase *)> dfunc = std::bind(static_cast<void(ModuleLocator::*)(modulebase::ModuleBase *)>(&ModuleLocator::DeleteObject_),
                                                                    this,
                                                                    std::placeholders::_1);

    removemap_.emplace(curid_, se.dfunc);

    // next id
    curid_++;

    return {mbptr, dfunc};
}


boost::python::object ModuleLocator::GetModulePy(const std::string & key)
{
    auto mod = CreateModule(key);
    return mod.first->MoveToPyObject_(mod.second);
}



ModuleLocator::~ModuleLocator()
{
    // it is the responsiblility of the various
    // loaders to be responsible for deleting all
    // the objects. The deletion functors aren't
    // called here since this should be destructed
    // after all the loaders
    //
    // Deleting after the loader had been destructed
    // probably wouldn't be a good idea, since the object
    // wouldn't exist anymore...
    //
    //! \todo Check if maps are empty?
}

} // close namespace modulelocator
} // close namespace bpmodule
