/*! \file
 *
 * \brief The graph holding the calculation data
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_DATASTORE__MODULEGRAPH_HPP_
#define BPMODULE_GUARD_DATASTORE__MODULEGRAPH_HPP_

#include <memory>

#include "bpmodule/datastore/graph/Graph.hpp"
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


//! \brief The graph containing module data
typedef LibGraph::Graph<ModuleGraphNode> ModuleGraph;
      






} // close namespace modulelocator
} // close namespace bpmodule


#endif
