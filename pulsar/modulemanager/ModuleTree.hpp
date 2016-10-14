/*! \file
 *
 * \brief A tree for storage of module information (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_MODULEMANAGER__MODULETREE_HPP_
#define PULSAR_GUARD_MODULEMANAGER__MODULETREE_HPP_

#include <map>
#include "pulsar/types.h"
#include "pulsar/modulemanager/ModuleTree_iterators.hpp"
#include "pulsar/modulemanager/ModuleInfo.hpp"

namespace pulsar{
namespace modulemanager {


/*! \brief Information stored on the tree for a module
 *
 * This should be considered read-only, except for the
 * module that uses this node, and the ModuleManager
 */
struct ModuleTreeNode
{
    /*
     * This is a simple structure, and the compiler-generated constructors, move
     * constructor, etc, should be OK
     */
    std::string modulekey;                //!< The key that this module was loaded with
    modulemanager::ModuleInfo minfo;      //!< The information for the module
    std::string output;                   //!< Output captured from the module

    ID_t id;                              //!< ID of the created module (also identifies this node)

    // tree stuff
    ID_t parentid;               //!< ID of the parent tree node
    std::set<ID_t> children;     //!< ID of the children of this node

    private:
        //! \name Serialization
        ///@{

        //! \todo hashing?

        DECLARE_SERIALIZATION_FRIENDS

        /* We have to split load/save since the
         * the shared_ptr points to const data, and
         * cereal can't serialize to const data
         */
        template<class Archive>
        void serialize(Archive & ar)
        {
            ar(modulekey, minfo, output, id);
            ar(parentid, children);
        }

        ///@}
};


/*! \brief A tree holding the information for the modules
 *
 * Whenever a new module is created, a node is added to the tree,
 * optionally with a specific parent. Therefore,
 * the tree stores the program flow and the data for each major
 * step of a calculation.
 */
class ModuleTree
{
    private:
        std::map<ID_t, ModuleTreeNode> data_;

    public:
        /// Depth-first iterator
        typedef detail::ConstModuleTreeIter const_iterator;

        /// Iterator over all elements
        typedef detail::ConstModuleFlatTreeIter const_flat_iterator;

        // non-copyable, but movable
        ModuleTree() = default;
        ModuleTree(ModuleTree &&) = default;
        ModuleTree & operator=(ModuleTree &&) = default;
        ModuleTree(const ModuleTree &) = delete;
        ModuleTree & operator=(const ModuleTree &) = delete;
        ~ModuleTree() = default;


        /*! \brief insert a node onto the tree
         *
         * \throw pulsar::ModuleManagerException if the
         *        parent node doesn't exist or if the id is
         *        not unique
         */
        void insert(ModuleTreeNode && node, ID_t parent);


        /*! \brief Query to see if the tree has a node with an ID
         */
        bool has_id(ID_t id) const;


        /*! \brief Obtain a node by its ID
         *
         * The ID represents the unique ID of the module
         * and also the unique ID of the tree node
         *
         * \throw pulsar::ModuleManagerException if
         *        a node with that ID doesn't exist
         */
        const ModuleTreeNode & get_by_id(ID_t id) const;


        /*! \brief Obtain a node by its ID
         *
         * The ID represents the unique ID of the module
         * and also the unique ID of the tree node
         *
         * \throw pulsar::ModuleManagerException if
         *        a node with that ID doesn't exist
         */
        ModuleTreeNode & get_by_id(ID_t id);


        /*! \brief Get the number of nodes in the tree */
        size_t size(void) const;


        /*! \brief Iterate (depth first) starting with a given ID
         *
         * \throw pulsar::ModuleManagerException if the
         *        a node with the starting id doesn't exist
         *
         * \param [in] startid ID to use as the base of the tree
         */
        const_iterator begin(ID_t startid) const;


        /*! \brief ending point of depth-first iteration */
        const_iterator end(void) const;


        /*! \brief Iterate over all modules in the tree
         *
         * The order is not specified, and should not be
         * assumed to be, for example, always increasing.
         */
        const_flat_iterator flat_begin(void) const;


        /*! \brief ending point of iteration over all modules in the tree */
        const_flat_iterator flat_end(void) const;

        /*! \brief Clear the contents of the entire tree */
        void clear(void);
};



} // close namespace modulemanager
} // close namespace pulsar


#endif
