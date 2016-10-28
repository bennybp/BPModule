/*! \file
 *
 * \brief Functions related to spherical transformations (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/system/SphericalTransform.hpp"



namespace pulsar{

namespace lut {
    extern const SphericalTransformMap spherical_transform_map_;
}


const SphericalTransformMap & all_spherical_transforms(void) noexcept
{
    return lut::spherical_transform_map_;
}


const std::vector<SphericalTransformCoefficient> & spherical_transform_for_am(int am)
{
    

    if(lut::spherical_transform_map_.count(am) == 0)
        throw GeneralException("Angular momentum out of range for spherical transform",
                               "am", am);

    return lut::spherical_transform_map_.at(am);
}

} // close namespace pulsar


