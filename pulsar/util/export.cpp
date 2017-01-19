/*! \file
 *
 * \brief Python exports for miscellaneous utilities
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include "pulsar/util/Pybind11.hpp"
#include "pulsar/util/Memwatch.hpp"


namespace pulsar{

void export_util(pybind11::module & m)
{
    //////////////////////////
    // Memwatch
    //////////////////////////
    m.def("memwatch_running", memwatch_running);
    m.def("memwatch_allocated", memwatch_allocated);
    m.def("memwatch_setlimit", memwatch_setlimit);
    m.def("memwatch_getlimit", memwatch_getlimit);
}

} // close namespace pulsar

