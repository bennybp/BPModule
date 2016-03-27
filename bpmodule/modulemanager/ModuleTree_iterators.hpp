/*! \file
 *
 * \brief Iterators over the module tree (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_MODULEMANAGER__MODULETREE_ITERATORS_HPP_
#define BPMODULE_GUARD_MODULEMANAGER__MODULETREE_ITERATORS_HPP_

#include <map>
#include <limits>

namespace bpmodule {
namespace modulemanager {


// Forward declare the tree for the iterators
class ModuleTree;
class ModuleTreeNode;


namespace detail {


/*! \brief A depth-first iterator through the tree
 */
class ConstModuleTreeIter
{
    private:
        friend class bpmodule::modulemanager::ModuleTree;
        static constexpr unsigned long MAXVAL = std::numeric_limits<unsigned long>::max();

        const ModuleTree * mtree_;
        unsigned long startid_;
        unsigned long curid_;
        const ModuleTreeNode * curnode_;

        ConstModuleTreeIter(const ModuleTree * mtree, unsigned long id);

    public:
        const ModuleTreeNode & operator*()const;

        const ModuleTreeNode * operator->()const;

        // Prefix
        ConstModuleTreeIter & operator++();

        // Postfix
        ConstModuleTreeIter operator++(int);

        // comparison
        bool operator==(const ConstModuleTreeIter & rhs) const;
        bool operator!=(const ConstModuleTreeIter & rhs) const;
};


/*! \brief An iterator through all nodes in an unspecified order
 */
class ConstModuleFlatTreeIter
{
    private:
        friend class bpmodule::modulemanager::ModuleTree;

        typedef std::map<unsigned long, ModuleTreeNode> MapType;

        const MapType * map_;
        MapType::const_iterator curit_;

        ConstModuleFlatTreeIter(const MapType * m, MapType::const_iterator it);

    public:
        const ModuleTreeNode & operator*()const;

        const ModuleTreeNode * operator->()const;

        // Prefix
        ConstModuleFlatTreeIter & operator++();

        // Postfix
        ConstModuleFlatTreeIter operator++(int);

        // comparison
        bool operator==(const ConstModuleFlatTreeIter & rhs) const;
        bool operator!=(const ConstModuleFlatTreeIter & rhs) const;
};


} // close namespace detail
} // close namespace modulemanager
} // close namespace bpmodule


#endif
