/*! \file
 *
 * \brief The graph holding the calculation data
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_GRAPH_HPP_
#define _GUARD_GRAPH_HPP_

#include "bpmodule/modulelocator/ModuleInfo.hpp"
#include "bpmodule/datastore/CalcData.hpp"


// forward declarations
namespace bpmodule {
namespace molecule {
class Molecule;
}

namespace basisset {
class BasisSet;
}
}




namespace bpmodule {
namespace modulelocator {

struct GraphNodeData
{
    molecule::Molecule * molecule;
    basisset::BasisSet * basisset;
    ModuleInfo minfo;
    datastore::CalcData data;
};


} // close namespace modulelocator
} // close namespace bpmodule


#endif
