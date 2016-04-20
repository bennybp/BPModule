/*! \file
 *
 * \brief Types of basis set shells
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_SYSTEM__SHELLTYPE_HPP_
#define PULSAR_GUARD_SYSTEM__SHELLTYPE_HPP_

#include <vector>
#include <stdexcept>
#include "pulsar/util/Serialization.hpp"


namespace pulsar{
namespace system {


/*! \brief Type of basis shells */
enum class ShellType
{
    SphericalGaussian,
    CartesianGaussian,
    Slater
};


inline const char * ShellTypeString(ShellType type)
{
    switch(type)
    {
        case ShellType::SphericalGaussian:
            return "SphericalGaussian";
        case ShellType::CartesianGaussian:
            return "CartesianGaussian";
        case ShellType::Slater:
            return "Slater";
        default:
            throw std::logic_error("Shell type to string is missing");
    }
};



} // close namespace system
} // close namespace pulsar


#endif
