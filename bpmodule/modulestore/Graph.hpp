/*! \file
 *
 * \brief The graph holding the calculation data
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_GRAPH_HPP_
#define _GUARD_GRAPH_HPP_

#include "bpmodule/modulestore/ModuleInfo.hpp"
#include "bpmodule/datastore/CalcData.hpp"

namespace bpmodule {
namespace modulestore {

namespace detail {

    struct GraphNodeData
    {
        //molecule
        //basis
        ModuleInfo minfo;
        datastore::CalcData data;
    };
}





} // close namespace modulestore
} // close namespace bpmodule


#endif
