/*! \file
 *
 * \brief A tree for storage of module information (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_MODULEMANAGER__MODULETREE_HPP_
#define BPMODULE_GUARD_MODULEMANAGER__MODULETREE_HPP_

#include <map>
#include "bpmodule/modulemanager/ModuleTree_iterators.hpp"
#include "bpmodule/modulemanager/ModuleInfo.hpp"
#include "bpmodule/datastore/Wavefunction.hpp"

namespace bpmodule {
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

    unsigned long id;                     //!< ID of the created module (also identifies this node)
    datastore::Wavefunction initial_wfn;  //!< Wavefunction this module started with
    datastore::Wavefunction final_wfn;    //!< Wavefunction this module ended with

    // tree stuff
    unsigned long parentid;               //!< ID of the parent tree node
    std::set<unsigned long> children;     //!< ID of the children of this node
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
        std::map<unsigned long, ModuleTreeNode> data_;


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


        /*! \brief Insert a node onto the tree
         *
         * \throw bpmodule::exception::ModuleManagerException if the
         *        parent node doesn't exist or if the id is
         *        not unique
         */
        void Insert(ModuleTreeNode && node, unsigned long parent);


        /*! \brief Query to see if the tree has a node with an ID
         */ 
        bool HasID(unsigned long id) const;


        /*! \brief Obtain a node by its ID
         * 
         * The ID represents the unique ID of the module
         * and also the unique ID of the tree node
         *
         * \throw bpmodule::exception::ModuleManagerException if
         *        a node with that ID doesn't exist
         */
        const ModuleTreeNode & GetByID(unsigned long id) const;


        /*! \brief Obtain a node by its ID
         * 
         * The ID represents the unique ID of the module
         * and also the unique ID of the tree node
         *
         * \throw bpmodule::exception::ModuleManagerException if
         *        a node with that ID doesn't exist
         */
        ModuleTreeNode & GetByID(unsigned long id);


        /*! \brief Get the number of nodes in the tree */
        size_t Size(void) const;


        /*! \brief Iterate (depth first) starting with a given ID
         * 
         * \throw bpmodule::exception::ModuleManagerException if the
         *        a node with the starting id doesn't exist
         *
         * \param [in] startid ID to use as the base of the tree
         */
        const_iterator Begin(unsigned long startid) const; 


        /*! \brief Ending point of depth-first iteration */
        const_iterator End(void) const; 


        /*! \brief Iterate over all modules in the tree
         *
         * The order is not specified, and should not be
         * assumed to be, for example, always increasing. 
         */
        const_flat_iterator FlatBegin(void) const; 

        
        /*! \brief Ending point of iteration over all modules in the tree */
        const_flat_iterator FlatEnd(void) const; 
};



} // close namespace modulemanager
} // close namespace bpmodule


#endif
