/*! \file
 *
 * \brief The graph holding the calculation data
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_DATASTORE__MODULEGRAPHNODE_HPP_
#define BPMODULE_GUARD_DATASTORE__MODULEGRAPHNODE_HPP_

#include <memory>

#include "bpmodule/modulemanager/ModuleInfo.hpp"
#include "bpmodule/datastore/Wavefunction.hpp"



namespace bpmodule {
namespace datastore {

struct ModuleGraphNodeData
{
    std::string modulekey;
    datastore::Wavefunction wfn;
    modulemanager::ModuleInfo minfo;
};


//! \todo will unique_ptr work?
typedef std::shared_ptr<ModuleGraphNodeData> ModuleGraphNode;



} // close namespace modulemanager
} // close namespace bpmodule


#endif
