/*! \file
 *
 * \brief Iterators over the module tree (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_MODULEMANAGER__MODULETREE_ITERATORS_HPP_
#define PULSAR_GUARD_MODULEMANAGER__MODULETREE_ITERATORS_HPP_

#include <map>
#include <limits>

namespace pulsar{
namespace modulemanager {


// Forward declare the tree for the iterators
class ModuleTree;
class ModuleTreeNode;


namespace detail {


/*! \brief A depth-first iterator through the tree
 *
 * Data stored on the nodes is not allowed to be changed
 * via this iterator (ie, it is a const_iterator)
 */
class ConstModuleTreeIter : public std::iterator<std::input_iterator_tag, 
                                                 const ModuleTreeNode>
{
    /* When this iterator represents the "end" of iteration,
     * startid_ = curid_ = MAXVAL (ie, the largest value for an ID_t)
     * and curnode_ = nullptr.
     */

    private:
        friend class pulsar::modulemanager::ModuleTree;
        static constexpr ID_t MAXVAL = std::numeric_limits<ID_t>::max();

        const ModuleTree * mtree_;       //!< The tree we are iterating over
        ID_t startid_;          //!< The ID of the node where we started iterating
        ID_t curid_;            //!< ID of the current node represented by the iterator
        const ModuleTreeNode * curnode_; //!< Pointer to the node data for the given curid_

        /*! \brief Constructor
         *
         * Only accessible by other objects of this class and by a ModuleTree
         *
         * \throw pulsar::exception::ModuleManagerException if mtree
         *        is a nullptr or the starting id doesn't exist
         *
         * \param [in] mtree The tree to interate over
         * \param [in] startid The id to start with
         */
        ConstModuleTreeIter(const ModuleTree * mtree, ID_t startid);


    public:
        ConstModuleTreeIter(const ConstModuleTreeIter &) = default;
        ConstModuleTreeIter(ConstModuleTreeIter &&) = default;
        ConstModuleTreeIter & operator=(const ConstModuleTreeIter &) = default;
        ConstModuleTreeIter & operator=(ConstModuleTreeIter &&) = default;

        /*! \brief Dereference the iterator
         *
         * \throw pulsar::exception::ModuleManagerException if the current
         *        node does not exist (ie, we are past the end)
         */
        const ModuleTreeNode & operator*()const;

        /*! \brief Dereference the iterator
         *
         * \throw pulsar::exception::ModuleManagerException if the current
         *        node does not exist (ie, we are past the end)
         */
        const ModuleTreeNode * operator->()const;

        /*! \brief Prefix increment */
        ConstModuleTreeIter & operator++();

        /*! \brief Postfix increment */
        ConstModuleTreeIter operator++(int);

        /*! \brief advance the iterator
         *
         * Similar to prefix increment. Mostly for use from python.
         */
        void advance(void);


        /*! \brief Get a reference to the node data
         *
         * Similar to dereferencing. Mostly for use from python
         *
         * \throw pulsar::exception::ModuleManagerException if the current
         *        node does not exist (ie, we are past the end)
         */
        const ModuleTreeNode & get_ref(void) const;


        // comparison
        bool operator==(const ConstModuleTreeIter & rhs) const;
        bool operator!=(const ConstModuleTreeIter & rhs) const;
};


/*! \brief An iterator through all nodes in an unspecified order
 *
 * Ordering of the nodes should never be assumed to be anything special.
 *
 * Data stored on the nodes is not allowed to be changed
 * via this iterator (ie, it is a const_iterator)
 */
class ConstModuleFlatTreeIter
{
    private:
        friend class pulsar::modulemanager::ModuleTree;

        typedef std::map<ID_t, ModuleTreeNode> MapType;

        const MapType * map_;            //!< The map we are iterating over
        MapType::const_iterator curit_;  //!< Iterator for the current node

        /*! \brief Constructor
         *
         * Only accessible by other objects of this class and by a ModuleTree
         *
         * \param [in] m The map to iterator over
         * \param [in] it Where to start the iteration
         */
        ConstModuleFlatTreeIter(const MapType * m, MapType::const_iterator it);

    public:
        ConstModuleFlatTreeIter(const ConstModuleFlatTreeIter &) = default;
        ConstModuleFlatTreeIter(ConstModuleFlatTreeIter &&) = default;
        ConstModuleFlatTreeIter & operator=(const ConstModuleFlatTreeIter &) = default;
        ConstModuleFlatTreeIter & operator=(ConstModuleFlatTreeIter &&) = default;

        /*! \brief Dereference the iterator
         *
         * \throw pulsar::exception::ModuleManagerException if the current
         *        node does not exist (ie, we are past the end)
         */
        const ModuleTreeNode & operator*()const;

        /*! \brief Dereference the iterator
         *
         * \throw pulsar::exception::ModuleManagerException if the current
         *        node does not exist (ie, we are past the end)
         */
        const ModuleTreeNode * operator->()const;

        /*! \brief Prefix increment */
        ConstModuleFlatTreeIter & operator++();

        /*! \brief Postfix increment */
        ConstModuleFlatTreeIter operator++(int);

        /*! \brief advance the iterator
         *
         * Similar to prefix increment. Mostly for use from python.
         */
        void advance(void);

        /*! \brief Get a reference to the node data
         *
         * Similar to dereferencing. Mostly for use from python
         *
         * \throw pulsar::exception::ModuleManagerException if the current
         *        node does not exist (ie, we are past the end)
         */
        const ModuleTreeNode & get_ref(void) const;

        // comparison
        bool operator==(const ConstModuleFlatTreeIter & rhs) const;
        bool operator!=(const ConstModuleFlatTreeIter & rhs) const;
};


} // close namespace detail
} // close namespace modulemanager
} // close namespace pulsar


#endif
