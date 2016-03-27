/*! \file
 *
 * \brief Iterators over the module tree (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#include "bpmodule/modulemanager/ModuleTree.hpp"

using bpmodule::exception::ModuleManagerException;


namespace bpmodule {
namespace modulemanager {
namespace detail {

///////////////////////////////////////
// ConstModuleTreeIter Iterator
///////////////////////////////////////
ConstModuleTreeIter::ConstModuleTreeIter(const ModuleTree * mtree, unsigned long id)
    : mtree_(mtree), startid_(id), curid_(id), curnode_(nullptr)
{
    // don't do the lookup if this is being constructed for "end"
    if(curid_ != MAXVAL)
        curnode_ = &mtree_->GetByID(curid_);
}


ConstModuleTreeIter & ConstModuleTreeIter::operator++()
{
    // does the current node have children?
    if(curnode_->children.size() > 0)
    {
        curid_ = *(curnode_->children.begin());
        curnode_ = &mtree_->GetByID(curid_);
        return *this;
    }
    else
    {
        // we reached the bottom. Go back up and
        // see if we have a sibling
        unsigned long parentid = curnode_->parentid;

        while(parentid != 0 && curid_ != startid_)
        {
            const ModuleTreeNode * parent = &mtree_->GetByID(parentid);

            unsigned long id = curid_; // for lambda

            // find a child with an ID greater than me
            const auto nextit = std::find_if(parent->children.begin(),
                                             parent->children.end(),
                                             [ id ] (unsigned long i) { return i > id; });

            if(nextit == parent->children.end())
            {
                // we've gone through all the children. Back up
                curid_ = parentid;
                curnode_ = parent;
                parentid = curnode_->parentid;
            }
            else
            {
                // we have a sibling!
                curid_ = *nextit;
                curnode_ = &mtree_->GetByID(curid_);
                return *this;
            }
        }

        // if we got here, parentid == 0
        // or curid_ == startid_
        // Either way, we are done
        curnode_ = nullptr;
        curid_ = startid_ = ConstModuleTreeIter::MAXVAL;
        return *this;
    }
}


ConstModuleTreeIter ConstModuleTreeIter::operator++(int)
{
    ConstModuleTreeIter ret(mtree_, curid_);
    ++(*this);
    return ret;
}
        

const ModuleTreeNode & ConstModuleTreeIter::operator*()const
{
    return *curnode_;
}

const ModuleTreeNode * ConstModuleTreeIter::operator->()const
{
    return curnode_;
}


bool ConstModuleTreeIter::operator==(const ConstModuleTreeIter & rhs) const
{
    return (curid_ == rhs.curid_ &&
            startid_ == rhs.startid_ &&
            mtree_ == rhs.mtree_);
}

bool ConstModuleTreeIter::operator!=(const ConstModuleTreeIter & rhs) const
{
    return !( (*this) == rhs);
}



///////////////////////////////////////
// ConstModuleFlatTreeIter Iterator
///////////////////////////////////////
ConstModuleFlatTreeIter::ConstModuleFlatTreeIter(const MapType *  m,
                                                 MapType::const_iterator it)
    : map_(m), curit_(it)
{
}

const ModuleTreeNode & ConstModuleFlatTreeIter::operator*()const
{
    return curit_->second;
}

const ModuleTreeNode * ConstModuleFlatTreeIter::operator->()const
{
    return &(curit_->second);
}

ConstModuleFlatTreeIter & ConstModuleFlatTreeIter::operator++()
{
    ++curit_;
    return *this;
}

ConstModuleFlatTreeIter ConstModuleFlatTreeIter::operator++(int)
{
    ConstModuleFlatTreeIter ret(map_, curit_);
    ++(*this);
    return ret;
}

bool ConstModuleFlatTreeIter::operator==(const ConstModuleFlatTreeIter & rhs) const
{
    return (map_ == rhs.map_ &&
            curit_ == rhs.curit_);
}

bool ConstModuleFlatTreeIter::operator!=(const ConstModuleFlatTreeIter & rhs) const
{
    return !( (*this) == rhs);
}


} // close namespace detail
} // close namespace modulemanager
} // close namespace bpmodule


