/*! \file
 *
 * \brief Python exports for the core Pulsar library
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include <pybind11/pybind11.h>

// helper for declaring submodule export functions
#define EXPORT_SUBMODULE(ns) namespace ns { \
              namespace export_python { \
                     void export_pybind11(pybind11::module &); \
              } }


namespace pulsar{

// declaration for all the various submodule exports
EXPORT_SUBMODULE(datastore)
EXPORT_SUBMODULE(exception)
EXPORT_SUBMODULE(math)
EXPORT_SUBMODULE(modulebase)
EXPORT_SUBMODULE(modulemanager)
EXPORT_SUBMODULE(output)
EXPORT_SUBMODULE(parallel)
EXPORT_SUBMODULE(python)
EXPORT_SUBMODULE(system)
EXPORT_SUBMODULE(tensor)
EXPORT_SUBMODULE(testing)
EXPORT_SUBMODULE(util)


////////////////////////////////////////
// Main export for pulsar
////////////////////////////////////////
namespace export_python {

PYBIND11_PLUGIN(pulsar_core)
{
    pybind11::module mtop("pulsar_core", "PulsarChem core");

    datastore::export_python::export_pybind11(mtop);
    exception::export_python::export_pybind11(mtop);
    math::export_python::export_pybind11(mtop);
    modulebase::export_python::export_pybind11(mtop);
    modulemanager::export_python::export_pybind11(mtop);
    output::export_python::export_pybind11(mtop);
    parallel::export_python::export_pybind11(mtop);
    python::export_python::export_pybind11(mtop);
    system::export_python::export_pybind11(mtop);
    tensor::export_python::export_pybind11(mtop);
    testing::export_python::export_pybind11(mtop);
    util::export_python::export_pybind11(mtop);

    return mtop.ptr();
}


} // close namespace export_python
} // close namespace pulsar

