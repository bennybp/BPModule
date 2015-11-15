/*! \file
 *
 * \brief The graph holding the calculation data
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_GRAPH_HPP_
#define _GUARD_GRAPH_HPP_

#include "bpmodule/modulelocator/ModuleInfo.hpp"
#include "bpmodule/datastore/CalcData.hpp"
#include "bpmodule/molecule/Molecule.hpp"
#include "bpmodule/basisset/BasisSet.hpp"

namespace bpmodule {
namespace modulelocator {

namespace detail {

    struct GraphNodeData
    {
        molecule::Molecule molecule;
        basisset::BasisSet basisset;
        ModuleInfo minfo;
        datastore::CalcData data;
    };
}





} // close namespace modulelocator
} // close namespace bpmodule


#endif
