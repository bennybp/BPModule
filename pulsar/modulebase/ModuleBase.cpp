/*! \file
 *
 * \brief The base class for all modules (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "pulsar/modulebase/ModuleBase.hpp"
#include "pulsar/datastore/Wavefunction.hpp"

using pulsar::modulemanager::ModuleManager;
using pulsar::modulemanager::ModuleInfo;
using pulsar::datastore::Wavefunction;
using pulsar::modulemanager::ModuleTreeNode;
using pulsar::datastore::CacheData;
using pulsar::datastore::OptionMap;
using pulsar::exception::GeneralException;


namespace pulsar{
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
    if(treenode_ == nullptr)
        out.Debug("Destroying a module that doesn't have a tree node...");
    else
    {
        out.Debug("Module [%?] : Output size: %?\n", ID(), GetOutput().size());
        out.Debug("Destructed module [%?] : %? v%?\n", ID(), Name(), Version());
        treenode_->inuse = false;
    }

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

OptionMap & ModuleBase::Options(void)
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

pybind11::object ModuleBase::CreateChildPy(const std::string & key) const
{
    return mlocator_->GetModulePy(key, id_);
}

pybind11::object ModuleBase::CreateChildFromOptionPy(const std::string & optionkey) const
{
    std::string modulekey = Options().Get<std::string>(optionkey);
    return mlocator_->GetModulePy(modulekey, id_);
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
} // close namespace pulsar
