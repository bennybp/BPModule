/*! \file
 *
 * \brief Types of basis set shells
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_SYSTEM__SHELLTYPE_HPP_
#define PULSAR_GUARD_SYSTEM__SHELLTYPE_HPP_

#include <vector>
#include "pulsar/util/Serialization.hpp"
#include "pulsar/exception/Assert.hpp"


namespace pulsar{
namespace system {


/*! \brief Type of basis shells */
enum class ShellType
{
    SphericalGaussian,
    CartesianGaussian,
    Slater
};


} // close namespace system
} // close namespace pulsar


#endif
