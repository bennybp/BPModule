/*! \file
 *
 * \brief The base class for all modules (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/modulebase/ModuleBase.hpp"
#include "bpmodule/datastore/Wavefunction.hpp"
#include "bpmodule/output/Output.hpp"

using bpmodule::modulelocator::ModuleManager;
using bpmodule::modulelocator::ModuleInfo;
using bpmodule::datastore::Wavefunction;
using bpmodule::datastore::ModuleGraphNodeData;
using bpmodule::datastore::ModuleGraphNode;
using bpmodule::datastore::CacheData;
using bpmodule::exception::GeneralException;


namespace bpmodule {
namespace modulebase {


ModuleBase::ModuleBase(unsigned long id, const char * modtype)
    : id_(id), modtype_(modtype), mlocator_(nullptr), graphnode_(nullptr)
{
    output::Debug("Constructed %1% module [%2%]\n", modtype, id);
}


ModuleBase::~ModuleBase()
{
    output::Debug("Destructed module [%1%] : %2% v%3%\n", ID(), Name(), Version());
}



/////////////////////////////
// Basic Info
/////////////////////////////
unsigned long ModuleBase::ID(void) const noexcept
{
    return id_;
}


std::string ModuleBase::Key(void) const 
{
    return MInfo_().key;
}


std::string ModuleBase::Name(void) const 
{
    return MInfo_().name;
}


std::string ModuleBase::Version(void) const 
{
    return MInfo_().version;
}


std::string ModuleBase::ModuleType(void) const
{
    return modtype_;
}

const datastore::OptionMap & ModuleBase::Options(void) const
{
    return MInfo_().options;
}

datastore::OptionMap & ModuleBase::Options(void)
{
    return MInfo_().options;
}


void ModuleBase::Print(void) const
{
    MInfo_().Print();
}

const ModuleGraphNode * ModuleBase::MyNode(void) const
{
    if(mlocator_ == nullptr)
        throw std::logic_error("Developer error - graphnode is null for a module!");

    return graphnode_;
}


////////////////////////////////
// Protected functions
////////////////////////////////
ModuleManager & ModuleBase::MManager(void) const
{
    if(mlocator_ == nullptr)
        throw std::logic_error("Developer error - mlocator is null for a module!");

    return *mlocator_;
}

const ModuleGraphNodeData & ModuleBase::GraphData(void) const
{
    if(graphnode_ == nullptr)
        throw std::logic_error("Developer error - graphnode_ is null for a module!");

    return *(*graphnode_);
}

ModuleGraphNodeData & ModuleBase::GraphData(void)
{
    if(graphnode_ == nullptr)
        throw std::logic_error("Developer error - graphnode_ is null for a module!");

    return *(*graphnode_);
}

const Wavefunction & ModuleBase::Wfn(void) const
{
    return GraphData().wfn;
}

Wavefunction & ModuleBase::Wfn(void)
{
    return GraphData().wfn;
}

pybind11::object ModuleBase::CreateChildModulePy(const std::string & key) const
{
    return mlocator_->GetModulePy(key, id_);
}

CacheData & ModuleBase::Cache(void) const noexcept
{
    if(cache_ == nullptr)
        throw std::logic_error("Developer error - cache_ is null for a module!");

    return *cache_;
}


////////////////////////////////
// Private functions
////////////////////////////////

void ModuleBase::SetMManager_(modulelocator::ModuleManager * mloc) noexcept
{
    mlocator_ = mloc;
}

void ModuleBase::SetGraphNode_(datastore::ModuleGraphNode * node) noexcept
{
    graphnode_ = node;
}

datastore::ModuleGraphNode * ModuleBase::GetGraphNode_(void) const noexcept
{
    return graphnode_;
}

void ModuleBase::SetCache_(datastore::CacheData * cache) noexcept
{
    cache_ = cache;
}

ModuleInfo & ModuleBase::MInfo_(void)
{
    return GraphData().minfo;
}

const ModuleInfo & ModuleBase::MInfo_(void) const
{
    return GraphData().minfo;
}


} // close namespace modulebase
} // close namespace bpmodule
