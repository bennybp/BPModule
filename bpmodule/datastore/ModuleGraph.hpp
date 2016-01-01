/*! \file
 *
 * \brief The graph holding the calculation data
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_DATASTORE__MODULEGRAPH_HPP_
#define BPMODULE_GUARD_DATASTORE__MODULEGRAPH_HPP_

#include <memory>

#include "bpmodule/datastore/graph/Graph.hpp"
#include "bpmodule/datastore/ModuleGraphNode.hpp"



namespace bpmodule {
namespace datastore {


/// instantiate once in the cpp file
//extern template class LibGraph::Graph<ModuleGraphNode>;


//! \brief The graph containing module data
typedef LibGraph::Graph<ModuleGraphNode> ModuleGraph;
      


} // close namespace modulelocator
} // close namespace bpmodule


#endif
