/*! \file
 *
 * \brief Dynamic library constructor/destructor
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 



/* These functions get run by the OS when loading or unloading
 * the shared library. This will happen at the very end, as python
 * itself is exiting.
 *
 * This is better than the atexit python module, since that may
 * run before final garbage collection and where some variables
 * may still be in scope
 */

#include "pulsar/pragma.h"
#include "pulsar/parallel/Parallel.hpp"
#include "pulsar/util/Cmdline.hpp"

PULSAR_PLUGIN_CONSTRUCTOR(pulsar_core_initialize_)
{
}

PULSAR_PLUGIN_DESTRUCTOR(pulsar_core_finalize_)
{
    pulsar::parallel_finalize();
    pulsar::clear_cmdline();
}
