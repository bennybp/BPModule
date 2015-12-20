/*! \file
 *
 * \brief Memory accounting
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "memwatch/memwatch.h"
#include "bpmodule/util/Memwatch.hpp"

namespace bpmodule {
namespace util {


bool Memwatch_running(void) noexcept
{
    return memwatch_running();
}

size_t Memwatch_allocated(void) noexcept
{
    return memwatch_query_allocated_memory();
}


size_t Memwatch_setlimit(size_t limit) noexcept
{
    return memwatch_set_max_memory(limit);
}

size_t Memwatch_getlimit(void) noexcept
{
    return memwatch_get_max_memory();
}


} // close namespace util
} // close namespace bpmodule

