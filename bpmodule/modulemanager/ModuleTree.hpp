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

    unsigned long moduleid;               //!< ID of the created module
    datastore::Wavefunction initial_wfn;  //!< Wavefunction this module started with
    datastore::Wavefunction final_wfn;    //!< Wavefunction this module ended with

    // tree stuff
    unsigned long parentid;               //!< ID of the parent tree node
    std::set<unsigned long> children;     //!< ID of the children of this node
};


/*! \brief A tree holding the information for the modules
 */
class ModuleTree
{
    private:
        std::map<unsigned long, ModuleTreeNode> data_;


    public:
        typedef detail::ConstModuleTreeIter const_iterator;
        typedef detail::ConstModuleFlatTreeIter const_flat_iterator;

        ModuleTree() = default;

        // non-copyable, but movable
        ModuleTree(ModuleTree &&) = default;
        ModuleTree & operator=(ModuleTree &&) = default;
        ModuleTree(const ModuleTree &) = delete;
        ModuleTree & operator=(const ModuleTree &) = delete;
        ~ModuleTree() = default;

        /*! \brief Insert a node onto the tree
         *
         * \throw bpmodule::exception::ModuleManagerException if the
         *        parent node doesn't exist or if the moduleid is
         *        not unique
         */
        void Insert(ModuleTreeNode && node, unsigned long parent);


        /*! \brief Query to see if the tree has a node with an ID
         */ 
        bool HasID(unsigned long id) const;


        const ModuleTreeNode & GetByID(unsigned long id) const;

        ModuleTreeNode & GetByID(unsigned long id);


        /*! \brief Get the number of nodes in the tree
         */ 
        size_t Size(void) const;


        /*! \brief Iterate starting with a given ID
         * 
         * \throw bpmodule::exception::ModuleManagerException if the
         *        node does not exist
         */
        const_iterator Begin(unsigned long startid) const; 

        /*! \brief Ending point of iteration
         */ 
        const_iterator End(void) const; 

        /*! \brief Iterate over all modules in the tree
         */
        const_flat_iterator FlatBegin(void) const; 

        /*! \brief Ending point of iteration
         */ 
        const_flat_iterator FlatEnd(void) const; 
};



} // close namespace modulemanager
} // close namespace bpmodule


#endif
