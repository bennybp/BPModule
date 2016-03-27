/*! \file
 *
 * \brief A tree for storage of module information (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "bpmodule/modulemanager/ModuleTree.hpp"

using bpmodule::exception::ModuleManagerException;


namespace bpmodule {
namespace modulemanager {



///////////////////////////////////////
// Module Tree
///////////////////////////////////////
void ModuleTree::Insert(ModuleTreeNode && node, unsigned long parentid)
{
    unsigned long mid = node.moduleid;
    bool hasparent = (parentid > 0);

    if(HasID(mid))
        throw ModuleManagerException("Duplicate module ID in the tree", "moduleid", mid);
    if(hasparent > 0 && !HasID(parentid))
        throw ModuleManagerException("Inserting a node with a non-existing parent", "parentid", parentid);

    node.parentid = parentid;
    data_.emplace(mid, std::move(node)); 

    // add as a child to the parent
    if(hasparent)
        data_.at(parentid).children.insert(mid);
}


bool ModuleTree::HasID(unsigned long id) const
{
    return data_.count(id);
}


const ModuleTreeNode & ModuleTree::GetByID(unsigned long id) const
{
    if(!HasID(id))
        throw ModuleManagerException("Module with this ID doesn't exist", "id", id);
    return data_.at(id);
}

ModuleTreeNode & ModuleTree::GetByID(unsigned long id)
{
    if(!HasID(id))
        throw ModuleManagerException("Module with this ID doesn't exist", "id", id);
    return data_.at(id);
}


size_t ModuleTree::Size(void) const
{
    return data_.size();
}

ModuleTree::const_iterator ModuleTree::Begin(unsigned long startid) const
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

} // close namespace modulemanager
} // close namespace bpmodule


