/*! \file
 *
 * \brief Functions related to spherical transformations (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "pulsar/system/SphericalTransform.hpp"

using namespace pulsar::exception;

namespace pulsar{
namespace system {




namespace lut {
    extern const SphericalTransformMap spherical_transform_map_;
}


const SphericalTransformMap & AllSphericalTransforms(void) noexcept
{
    return lut::spherical_transform_map_;
}



} // close namespace system
} // close namespace pulsar


