/*! \file
 *
 * \brief The graph holding the calculation data
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_GRAPHNODEDATA_HPP_
#define _GUARD_GRAPHNODEDATA_HPP_

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


} // close namespace modulelocator
} // close namespace bpmodule


#endif
