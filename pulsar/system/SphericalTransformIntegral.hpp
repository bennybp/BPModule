/*! \file
 *
 * \brief Herlpers for spherical transformation of integrals
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_SYSTEM__SPHERICALTRANSFORMINTEGRAL_HPP_
#define PULSAR_GUARD_SYSTEM__SPHERICALTRANSFORMINTEGRAL_HPP_


#include "pulsar/system/SphericalTransform.h"

namespace pulsar{
namespace system {


/*! \brief Transforms one-electron integrals over gaussian basis functions
 *
 * The integrals should all be in cartesian and correspond to the integrals
 * for sh1 and sh2. Whether each is transformed depends on the type
 * stored in sh1 and sh2.
 *
 * \p source size should be the number of cartesian integrals
 * (taking into account general contractions). Work should also be
 * that size.
 *
 * \p dest should be big enough to hold the transformed integrals.
 * This depends on if sh1 and sh2 are spherical or not. For simplicity,
 * this could be as big as \p source.
 *
 * Cartesian ordering is expected to be pulsar ordering.
 */
void CartesianToSpherical_OneElectron(const system::BasisSetShell & sh1,
                                      const system::BasisSetShell & sh2,
                                      double const * RESTRICT source,
                                      double * RESTRICT dest,
                                      double * RESTRICT work)
{
    using namespace system;

    bool isspherical1 = sh1.GetType() == ShellType::SphericalGaussian;
    bool isspherical2 = sh2.GetType() == ShellType::SphericalGaussian;

    if(!isspherical1 && !isspherical2)
    {
        // you probably shouldn't have even called this function
        std::copy(source, source + sh1.NFunctions()*sh2.NFunctions(), dest);
        return;
    }

    size_t ngen1 = sh1.NGeneral();
    size_t ngen2 = sh2.NGeneral();

    for(size_t ng1 = 0; ng1 < ngen1; ng1++)
    for(size_t ng2 = 0; ng2 < ngen2; ng2++)
    {
        const int am1 = sh1.GeneralAM(ng1);
        const int am2 = sh2.GeneralAM(ng2);

        const size_t ncart1 = NCartesianGaussian(am1);
        const size_t ncart2 = NCartesianGaussian(am2);
        const size_t ncart12 = ncart1 * ncart2;

        const size_t nsph1 = NSphericalGaussian(am1);
        const size_t nsph2 = NSphericalGaussian(am2);
        const size_t nsph12 = nsph1 * nsph2;

        if(isspherical1 && isspherical2)
        {
            std::fill(work, work + nsph1*ncart2, 0.0);
            std::fill(dest, dest + nsph12, 0.0);
            const auto & coef1 = SphericalTransformForAM(am1);
            const auto & coef2 = SphericalTransformForAM(am2);

            // we have to use the workspace
            // transform first index
            SphericalTransformBlock(coef1, source, work, ncart2, am1, 1);

            // second index
            SphericalTransformBlock(coef2, work, dest, 1, am2, nsph1);

            dest += nsph12;
        }
        else if(isspherical1)
        {
            std::fill(dest, dest + nsph1*ncart2, 0.0);

            // transform first index right into dest
            const auto & coef1 = SphericalTransformForAM(am1);
            SphericalTransformBlock(coef1, source, dest, ncart2, am1, 1);

            dest += nsph1*ncart2;
        }
        else
        {
            std::fill(dest, dest + ncart1*nsph2, 0.0);

            // transform second index right into dest
            const auto & coef2 = SphericalTransformForAM(am2);
            SphericalTransformBlock(coef2, source, dest, 1, am2, ncart1);

            dest += nsph2*ncart1;
        }

        source += ncart12;
    }
}


} // close namespace system
} // close namespace pulsar


#endif
