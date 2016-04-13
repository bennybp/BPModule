/*! \file
 *
 * \brief The base class for all modules (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/modulebase/ModuleBase.hpp"
#include "bpmodule/datastore/Wavefunction.hpp"

using bpmodule::modulemanager::ModuleManager;
using bpmodule::modulemanager::ModuleInfo;
using bpmodule::datastore::Wavefunction;
using bpmodule::modulemanager::ModuleTreeNode;
using bpmodule::datastore::CacheData;
using bpmodule::datastore::OptionMap;
using bpmodule::exception::GeneralException;


namespace bpmodule {
namespace modulebase {


ModuleBase::ModuleBase(ID_t id, const char * modtype)
    : tbts_(std::cout.rdbuf(), nullptr),  // by default, go to cout only
      out(&tbts_),
      id_(id), modtype_(modtype), mlocator_(nullptr), treenode_(nullptr)
{
    out.Debug("Constructed %? module [%?]\n", modtype, id);
}


ModuleBase::~ModuleBase()
{
    out.Debug("Module [%?] : Output size: %?\n", ID(), GetOutput().size());
    out.Debug("Destructed module [%?] : %? v%?\n", ID(), Name(), Version());

    if(mlocator_ == nullptr)
        out.Error("Module module locator is null");
    else
        mlocator_->DestructionNotify(ID());

    //out.Debug("*****\n");
    //out.Warning(GetOutput());
    //out.Debug("*****\n");
}



/////////////////////////////
// Basic Info
/////////////////////////////
ID_t ModuleBase::ID(void) const noexcept
{
    return id_;
}


std::string ModuleBase::Key(void) const 
{
    return MyNode().modulekey;
}


std::string ModuleBase::Name(void) const 
{
    return MyNode().minfo.name;
}


std::string ModuleBase::Version(void) const 
{
    return MyNode().minfo.version;
}


std::string ModuleBase::ModuleType(void) const
{
    return modtype_;
}

const OptionMap & ModuleBase::Options(void) const
{
    return MyNode().minfo.options;
}

void ModuleBase::Print(std::ostream & os) const
{
    MyNode().minfo.Print(os);
}


void ModuleBase::EnableDebug(bool debug) noexcept
{
    out.EnableDebug(debug);
}

bool ModuleBase::DebugEnabled(void) const noexcept
{
    return out.DebugEnabled();
}

const ModuleTreeNode & ModuleBase::MyNode(void) const
{
    if(treenode_ == nullptr)
        throw std::logic_error("Developer error - tree node is null for a module!");
    return *treenode_;
}

Wavefunction & ModuleBase::InitialWfn(void)
{
    return MyNode().initial_wfn;
}

const Wavefunction & ModuleBase::InitialWfn(void) const
{
    return MyNode().initial_wfn;
}


void ModuleBase::SetInitialWfn(const Wavefunction& wfn)
{
    MyNode().initial_wfn=wfn;
}

Wavefunction & ModuleBase::FinalWfn(void)
{
    return MyNode().final_wfn;
}

const Wavefunction & ModuleBase::FinalWfn(void) const
{
    return MyNode().final_wfn;
}

void ModuleBase::SetFinalWfn(const Wavefunction& wfn)
{
    MyNode().final_wfn=wfn;
}


std::string ModuleBase::GetOutput(void) const
{
    return MyNode().output;
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

void ModuleBase::SetMManager_(ModuleManager * mloc) noexcept
{
    mlocator_ = mloc;
}

void ModuleBase::SetTreeNode_(ModuleTreeNode * node) noexcept
{
    if(node == nullptr)
        throw std::logic_error("Developer error - module given a nullptr for tree node!");

    treenode_ = node;

    // tee the output to the tree
    tbts_.SetString(&(MyNode().output));
}

ModuleTreeNode & ModuleBase::MyNode(void)
{
    if(treenode_ == nullptr)
        throw std::logic_error("Developer error - tree node is null for a module!");
    return *treenode_;
}

void ModuleBase::SetCache_(CacheData * cache) noexcept
{
    cache_ = cache;
}

} // close namespace modulebase
} // close namespace bpmodule
