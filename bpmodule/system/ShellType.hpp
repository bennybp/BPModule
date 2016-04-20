/*! \file
 *
 * \brief Types of basis set shells
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef BPMODULE_GUARD_SYSTEM__SHELLTYPE_HPP_
#define BPMODULE_GUARD_SYSTEM__SHELLTYPE_HPP_

#include <vector>
#include "bpmodule/util/Serialization.hpp"
#include "bpmodule/exception/Assert.hpp"


namespace bpmodule {
namespace system {


/*! \brief Type of basis shells */
enum class ShellType
{
    SphericalGaussian,
    CartesianGaussian,
    Slater
};


} // close namespace system
} // close namespace bpmodule


#endif
