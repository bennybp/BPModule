/*! \file
 *
 * \brief A tree for storage of module information (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "pulsar/modulemanager/ModuleTree.hpp"

using pulsar::exception::ModuleManagerException;


namespace pulsar{
namespace modulemanager {



///////////////////////////////////////
// Module Tree
///////////////////////////////////////
void ModuleTree::Insert(ModuleTreeNode && node, ID_t parentid)
{
    ID_t mid = node.id;
    bool hasparent = (parentid > 0);

    if(HasID(mid))
        throw ModuleManagerException("Duplicate ID in the tree", "id", mid);
    if(hasparent > 0 && !HasID(parentid))
        throw ModuleManagerException("Inserting a node with a non-existing parent", "parentid", parentid);

    node.parentid = parentid;
    data_.emplace(mid, std::move(node));

    // add as a child to the parent
    if(hasparent)
        data_.at(parentid).children.insert(mid);
}


bool ModuleTree::HasID(ID_t id) const
{
    return data_.count(id);
}


const ModuleTreeNode & ModuleTree::GetByID(ID_t id) const
{
    if(!HasID(id))
        throw ModuleManagerException("Module with this ID doesn't exist", "id", id);
    return data_.at(id);
}

ModuleTreeNode & ModuleTree::GetByID(ID_t id)
{
    if(!HasID(id))
        throw ModuleManagerException("Module with this ID doesn't exist", "id", id);
    return data_.at(id);
}


size_t ModuleTree::Size(void) const
{
    return data_.size();
}

ModuleTree::const_iterator ModuleTree::Begin(ID_t startid) const
{
    if(!HasID(startid))
        throw ModuleManagerException("Module with this ID doesn't exist", "startid", startid);
    return const_iterator(this, startid);
}

ModuleTree::const_iterator ModuleTree::End(void) const
{
    return const_iterator(this, const_iterator::MAXVAL);
}

ModuleTree::const_flat_iterator ModuleTree::FlatBegin(void) const
{
    return const_flat_iterator(&data_, data_.begin());
}

ModuleTree::const_flat_iterator ModuleTree::FlatEnd(void) const
{
    return const_flat_iterator(&data_, data_.end());
}

void ModuleTree::Clear(void)
{
    data_.clear();
}

} // close namespace modulemanager
} // close namespace pulsar


