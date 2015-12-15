/*! \file
 *
 * \brief Main module database class (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "bpmodule/modulelocator/ModuleLocator.hpp"
#include "bpmodule/output/Output.hpp"
#include "bpmodule/modulebase/ModuleBase.hpp"
#include "bpmodule/exception/ModuleLoadException.hpp"
#include "bpmodule/datastore/Wavefunction.hpp"

//////////////////////////////////////////////
//! \todo can probably be removed when graph inheritance is done
#include "bpmodule/basisset/BasisSet.hpp"
#include "bpmodule/molecule/Molecule.hpp"
#include "bpmodule/tensor/Matrix.hpp"
using bpmodule::molecule::Molecule;
using bpmodule::basisset::BasisSet;
using bpmodule::tensor::DistMatrixD;
//////////////////////////////////////////////


using bpmodule::datastore::GraphNodeData;
using bpmodule::datastore::GraphNode;
using bpmodule::datastore::Wavefunction;
using bpmodule::modulebase::ModuleBase;
using bpmodule::exception::ModuleLoadException;
using bpmodule::exception::ModuleLocatorException;
using bpmodule::exception::GeneralException;


namespace bpmodule {
namespace modulelocator {



ModuleLocator::ModuleLocator()
    : curid_(100) // reserve some for my use
{ }


ModuleLocator::~ModuleLocator()
{
    //! \todo Check if maps are empty?
}



void ModuleLocator::InsertModule(const std::string & key, const ModuleCreatorFunc & mc, const ModuleInfo & mi)
{
    // add to store
    // but throw if key already exists
    if(store_.count(key))
        throw ModuleLoadException("Cannot insert module: duplicate key",
                                  mi.path, key, mi.name);

    store_.emplace(key, StoreEntry{mi, mc});
}


void ModuleLocator::SetOptions(const std::string & key, pybind11::dict opt)
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
            GetModule<ModuleBase>(it.first, 0);
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


void ModuleLocator::ClearCache(void)
{
    cachemap_.clear();
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


/////////////////////////////////////////
// Module Creation
/////////////////////////////////////////
std::unique_ptr<detail::ModuleIMPLHolder>
ModuleLocator::CreateModule_(const std::string & key, unsigned long parentid)
{
    // obtain the creator
    const StoreEntry & se = GetOrThrow_(key);

    // create
    detail::ModuleIMPLHolder * mbptr;
    try {
      mbptr = se.mc(curid_);
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

    // wrap in a unique_ptr
    std::unique_ptr<detail::ModuleIMPLHolder> umbptr(mbptr);
   
 
    // add the moduleinfo to the graph
    //! \todo replace with actual graph
    if(parentid != 0)
    {
        GraphNode & parent = graphnodes_.at(parentid);
        GraphNodeData gdata{parent.data.wfn, se.mi};
        graphnodes_.emplace(curid_, GraphNode{gdata});
    }
    else
    {
        GraphNodeData gdata{Wavefunction(), se.mi};
        graphnodes_.emplace(curid_, GraphNode{gdata});
    }

    // set the info
    ModuleBase * p = umbptr->CppPtr();

    p->SetMLocator_(this);
    p->SetGraphNode_(&(graphnodes_.at(curid_)));

    // get this modules cache
    // no need to use .at() -- we need it created if it doesn't exist already
    std::string mbstr = p->Name() + "_v" + p->Version();
    p->SetCache_(&(cachemap_[mbstr]));

    // next id
    curid_++;

    return std::move(umbptr);
}


////////////////////
// Python
////////////////////
pybind11::object ModuleLocator::GetModulePy(const std::string & key, unsigned long parentid)
{
    auto umbptr = CreateModule_(key, parentid);
    return pybind11::cast(PyModulePtr(std::move(umbptr)));
}


} // close namespace modulelocator
} // close namespace bpmodule
