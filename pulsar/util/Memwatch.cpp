/*! \file
 *
 * \brief Memory accounting
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "memwatch/memwatch.h"
#include "pulsar/util/Memwatch.hpp"

namespace pulsar{

bool memwatch_running(void) noexcept
{
    return ::memwatch_running();
}

size_t memwatch_allocated(void) noexcept
{
    return ::memwatch_query_allocated_memory();
}


size_t memwatch_setlimit(size_t limit) noexcept
{
    return ::memwatch_set_max_memory(limit);
}

size_t memwatch_getlimit(void) noexcept
{
    return ::memwatch_get_max_memory();
}

} // close namespace pulsar

