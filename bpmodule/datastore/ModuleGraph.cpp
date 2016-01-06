/*! \file
 *
 * \brief The graph holding the calculation data
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "bpmodule/datastore/graph/Graph.hpp"
#include "bpmodule/datastore/ModuleGraphNode.hpp"


namespace bpmodule {
namespace datastore {


/// instantiate once here
template class LibGraph::Graph<ModuleGraphNode>;



} // close namespace modulelocator
} // close namespace bpmodule

