/*! \file
 *
 * \brief The graph holding the calculation data
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_DATASTORE__MODULEGRAPHNODE_HPP_
#define BPMODULE_GUARD_DATASTORE__MODULEGRAPHNODE_HPP_

#include <memory>

#include "bpmodule/modulelocator/ModuleInfo.hpp"
#include "bpmodule/datastore/Wavefunction.hpp"



namespace bpmodule {
namespace datastore {

struct ModuleGraphNodeData
{
    datastore::Wavefunction wfn;
    modulelocator::ModuleInfo minfo;
};


//! \todo will unique_ptr work?
typedef std::shared_ptr<ModuleGraphNodeData> ModuleGraphNode;



} // close namespace modulelocator
} // close namespace bpmodule


#endif
