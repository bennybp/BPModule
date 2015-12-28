/*! \file
 *
 * \brief The graph holding the calculation data
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_DATASTORE__GRAPHNODEDATA_HPP_
#define BPMODULE_GUARD_DATASTORE__GRAPHNODEDATA_HPP_

#include <memory>

#include "bpmodule/modulelocator/ModuleInfo.hpp"
#include "bpmodule/datastore/Wavefunction.hpp"


namespace bpmodule {
namespace datastore {

struct GraphNodeData
{
    datastore::Wavefunction wfn;
    modulelocator::ModuleInfo minfo;
};

//! \todo Replace with actual graph node
struct GraphNode
{
    GraphNodeData data;
};


} // close namespace modulelocator
} // close namespace bpmodule


#endif
