/*! \file
 *
 * \brief Python exports for the core Pulsar library
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include <pybind11/pybind11.h>

namespace pulsar{

// declaration for all the various submodule exports
void export_datastore(pybind11::module &);
void export_exception(pybind11::module &);
void export_math(pybind11::module &);
void export_modulebase(pybind11::module &);
void export_modulemanager(pybind11::module &);
void export_output(pybind11::module &);
void export_parallel(pybind11::module &);
void export_python(pybind11::module &);
void export_system(pybind11::module &);
void export_testing(pybind11::module &);
void export_util(pybind11::module &);


////////////////////////////////////////
// Main export for pulsar
////////////////////////////////////////
PYBIND11_PLUGIN(pulsar_core)
{
    pybind11::module mtop("pulsar_core", "PulsarChem core");

    export_datastore(mtop);
    export_exception(mtop);
    export_math(mtop);
    export_modulebase(mtop);
    export_modulemanager(mtop);
    export_output(mtop);
    export_parallel(mtop);
    export_python(mtop);
    export_system(mtop);
    export_testing(mtop);
    export_util(mtop);

    return mtop.ptr();
}
} // close namespace pulsar


#undef EXPORT_SUBMODULE
