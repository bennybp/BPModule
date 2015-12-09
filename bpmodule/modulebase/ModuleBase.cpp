/*! \file
 *
 * \brief The base class for all modules (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/modulebase/ModuleBase.hpp"
#include "bpmodule/modulelocator/ModuleLocator.hpp"
#include "bpmodule/datastore/Wavefunction.hpp"
#include "bpmodule/output/Output.hpp"

using bpmodule::modulelocator::ModuleLocator;
using bpmodule::modulelocator::ModuleInfo;
using bpmodule::datastore::Wavefunction;
using bpmodule::datastore::GraphNodeData;
using bpmodule::datastore::GraphNode;
using bpmodule::exception::GeneralException;


namespace bpmodule {
namespace modulebase {


ModuleBase::ModuleBase(unsigned long id)
    : pyself_(nullptr), id_(id), mlocator_(nullptr), graphnode_(nullptr)
{
    output::Debug("Constructed module [%1%]\n", id);
}

ModuleBase::ModuleBase(PyObject * self, unsigned long id)
    : pyself_(self), id_(id), mlocator_(nullptr), graphnode_(nullptr)
{
    output::Debug("Constructed python module [%1%]\n", id);
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


const options::OptionMap & ModuleBase::Options(void) const
{
    return MInfo_().options;
}

options::OptionMap & ModuleBase::Options(void)
{
    return MInfo_().options;
}


void ModuleBase::Print(void) const
{
    MInfo_().Print();
}

bool ModuleBase::IsPythonModule(void) const noexcept
{
    return (pyself_ != nullptr);
}


const GraphNode * ModuleBase::MyNode(void) const
{
    if(mlocator_ == nullptr)
        throw std::logic_error("Developer error - graphnode is null for a module!");

    return graphnode_;
}


////////////////////////////////
// Protected functions
////////////////////////////////
ModuleLocator & ModuleBase::MLocator(void) const
{
    if(mlocator_ == nullptr)
        throw std::logic_error("Developer error - mlocator is null for a module!");

    return *mlocator_;
}

const GraphNodeData & ModuleBase::GraphData(void) const
{
    if(graphnode_ == nullptr)
        throw std::logic_error("Developer error - graphnode_ is null for a module!");

    return graphnode_->data;
}

GraphNodeData & ModuleBase::GraphData(void)
{
    if(graphnode_ == nullptr)
        throw std::logic_error("Developer error - graphnode_ is null for a module!");

    return graphnode_->data;
}

const Wavefunction & ModuleBase::Wfn(void) const
{
    return GraphData().wfn;
}

Wavefunction & ModuleBase::Wfn(void)
{
    return GraphData().wfn;
}

boost::python::object ModuleBase::CreateChildModulePy(const std::string & key) const
{
    return mlocator_->GetModulePy(key, id_);
}


////////////////////////////////
// Private functions
////////////////////////////////

void ModuleBase::SetMLocator_(modulelocator::ModuleLocator * mloc) noexcept
{
    mlocator_ = mloc;
}

void ModuleBase::SetGraphNode_(datastore::GraphNode * node) noexcept
{
    graphnode_ = node;
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
