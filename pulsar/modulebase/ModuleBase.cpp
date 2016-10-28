/*! \file
 *
 * \brief The base class for all modules (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "pulsar/modulebase/ModuleBase.hpp"
#include "pulsar/datastore/Wavefunction.hpp"
#include "pulsar/output/GlobalOutput.hpp"

using pulsar::ModuleManager;
using pulsar::ModuleInfo;
using pulsar::ModuleTreeNode;


namespace pulsar{


ModuleBase::ModuleBase(ID_t id, const char * modtype)
    : tbts_(get_global_output().rdbuf(), nullptr),
      out(&tbts_),
      id_(id), modtype_(modtype), mlocator_(nullptr),
      treenode_(nullptr)
{
    out.debug("Constructed %? module [%?]\n", modtype, id);
}


ModuleBase::~ModuleBase()
{
    if(treenode_ == nullptr)
        out.debug("Destroying a module that doesn't have a tree node...");
    else
    {
        out.debug("Module [%?] : Output size: %?\n", id(), get_output().size());
        out.debug("Destructed module [%?] : %? v%?\n", id(), name(), version());
        mlocator_->notify_destruction(id());
    }
}



/////////////////////////////
// Basic Info
/////////////////////////////
ID_t ModuleBase::id(void) const noexcept
{
    return id_;
}


std::string ModuleBase::key(void) const
{
    return my_node().modulekey;
}


std::string ModuleBase::name(void) const
{
    return my_node().minfo.name;
}


std::string ModuleBase::version(void) const
{
    return my_node().minfo.version;
}


std::string ModuleBase::module_type(void) const
{
    return modtype_;
}

const OptionMap & ModuleBase::options(void) const
{
    return my_node().minfo.options;
}

OptionMap & ModuleBase::options(void)
{
    return my_node().minfo.options;
}

void ModuleBase::print(std::ostream & os) const
{
    my_node().minfo.print(os);
}


void ModuleBase::enable_debug(bool debug) noexcept
{
    out.enable_debug(debug);
}

bool ModuleBase::debug_enabled(void) const noexcept
{
    return out.debug_enabled();
}

const ModuleTreeNode & ModuleBase::my_node(void) const
{
    if(treenode_ == nullptr)
        throw std::logic_error("Developer error - tree node is null for a module!");
    return *treenode_;
}

std::string ModuleBase::get_output(void) const
{
    return my_node().output;
}



////////////////////////////////
// Protected functions
////////////////////////////////
ModuleManager & ModuleBase::module_manager(void) const
{
    if(mlocator_ == nullptr)
        throw std::logic_error("Developer error - mlocator is null for a module!");

    return *mlocator_;
}

pybind11::object ModuleBase::create_child_py(const std::string & key) const
{
    return mlocator_->get_module_py(key, id_);
}

pybind11::object ModuleBase::create_child_from_option_py(const std::string & optionkey) const
{
    std::string modulekey = options().get<std::string>(optionkey);
    return mlocator_->get_module_py(modulekey, id_);
}

CacheData & ModuleBase::cache(void) const noexcept
{
    if(!cache_)
        throw std::logic_error("Developer error - cache_ is null for a module!");

    return *cache_;
}


////////////////////////////////
// Private functions
////////////////////////////////

void ModuleBase::set_module_manager_(ModuleManager * mloc) noexcept
{
    mlocator_ = mloc;
}

void ModuleBase::set_tree_node_(ModuleTreeNode * node) noexcept
{
    if(node == nullptr)
        throw std::logic_error("Developer error - module given a nullptr for tree node!");

    treenode_ = node;

    // tee the output to the tree
    tbts_.set_string(&(my_node().output));
}

ModuleTreeNode & ModuleBase::my_node(void)
{
    if(treenode_ == nullptr)
        throw std::logic_error("Developer error - tree node is null for a module!");
    return *treenode_;
}

void ModuleBase::set_cache_(CacheData && cache)
{
    cache_ = std::unique_ptr<CacheData>(new CacheData(std::move(cache)));
}

} // close namespace pulsar
