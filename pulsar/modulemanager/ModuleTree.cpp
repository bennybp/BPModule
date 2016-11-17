/*! \file
 *
 * \brief A tree for storage of module information (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "pulsar/modulemanager/ModuleTree.hpp"

using pulsar::PulsarException;


namespace pulsar{

///////////////////////////////////////
// Module Tree
///////////////////////////////////////
void ModuleTree::insert(ModuleTreeNode && node, ID_t parentid)
{
    ID_t mid = node.id;
    bool hasparent = (parentid > 0);

    if(has_id(mid))
        throw PulsarException("Duplicate ID in the tree", "id", mid);
    if(hasparent > 0 && !has_id(parentid))
        throw PulsarException("inserting a node with a non-existing parent", "parentid", parentid);

    node.parentid = parentid;
    data_.emplace(mid, std::move(node));

    // add as a child to the parent
    if(hasparent)
        data_.at(parentid).children.insert(mid);
}


bool ModuleTree::has_id(ID_t id) const
{
    return data_.count(id);
}


const ModuleTreeNode & ModuleTree::get_by_id(ID_t id) const
{
    if(!has_id(id))
        throw PulsarException("Module with this ID doesn't exist", "id", id);
    return data_.at(id);
}

ModuleTreeNode & ModuleTree::get_by_id(ID_t id)
{
    if(!has_id(id))
        throw PulsarException("Module with this ID doesn't exist", "id", id);
    return data_.at(id);
}


size_t ModuleTree::size(void) const
{
    return data_.size();
}

ModuleTree::const_iterator ModuleTree::begin(ID_t startid) const
{
    if(!has_id(startid))
        throw PulsarException("Module with this ID doesn't exist", "startid", startid);
    return const_iterator(this, startid);
}

ModuleTree::const_iterator ModuleTree::end(void) const
{
    return const_iterator(this, const_iterator::MAXVAL);
}

ModuleTree::const_flat_iterator ModuleTree::flat_begin(void) const
{
    return const_flat_iterator(&data_, data_.begin());
}

ModuleTree::const_flat_iterator ModuleTree::flat_end(void) const
{
    return const_flat_iterator(&data_, data_.end());
}

void ModuleTree::clear(void)
{
    data_.clear();
}

} // close namespace pulsar


