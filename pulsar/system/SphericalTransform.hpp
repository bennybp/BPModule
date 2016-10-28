/*! \file
 *
 * \brief Functions related to spherical transformations (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_SYSTEM__SPHERICALTRANSFORM_HPP_
#define PULSAR_GUARD_SYSTEM__SPHERICALTRANSFORM_HPP_

#include <map>
#include <vector>
#include <complex>

#include "pulsar/system/BasisSet.hpp"
#include "pulsar/system/NFunction.hpp"

namespace pulsar{

//! transformation coefficient of a cartesian gaussian
struct SphericalTransformCoefficient
{
    int sphidx;
    int cartidx;
    double coef;
};



/*! Spherical transformation for a singular angular momentum */
typedef std::vector<SphericalTransformCoefficient> SphericalTransformCoefs;


//! A map for spherical transformations
typedef std::map<int, SphericalTransformCoefs> SphericalTransformMap;



/*! \brief Obtain the spherical transformations used by pulsar */
const SphericalTransformMap & all_spherical_transforms(void) noexcept;


/*! \brief Obtain the spherical transformations used by pulsar for
 *         for a specific angular momentum
 */
const std::vector<SphericalTransformCoefficient> & spherical_transform_for_am(int am);


/*! \brief transform a block of cartesian AO data to spherical harmonics
 *
 * \warning \p src and \p dest are expected to be the correct sizes. It is up
 *          to the caller to make sure. The size of \p src should be
 *          ncartesian(am) * width * niter. The size of \p dest should be
 *          nspherical(am) * width * niter
 *
 * \warning \p src and \p dest must not point to the same location
 *
 * \todo restrict keyword
 */
template<typename T>
inline void spherical_transform_block(const SphericalTransformCoefs & coefs,
                                    T const * RESTRICT src,
                                    T * RESTRICT dest,
                                    size_t width, int am, size_t niter)
{
    const size_t ncart = n_cartesian_gaussian(am);
    const size_t nsph = n_spherical_gaussian(am);

    std::fill(dest, dest + width*niter*nsph, 0.0);

    for(size_t n = 0; n < niter; n++)
    {   
        for(const auto & it : coefs)
        {
            const size_t sph_start = it.sphidx * width;
            const size_t cart_start = it.cartidx * width;

            for(size_t i = 0; i < width; i++)
                dest[sph_start + i] += it.coef * src[cart_start + i];
        }

        src += ncart*width;
        dest += nsph*width;
    }
}



/*! \brief transform data from cartesian to spherical gaussian functions
 *
 * It is expected that all of \p src is in cartesian form. If the corresponding
 * shells in the basis set are spherical gaussians, the block is converted
 * to spherical harmonics. If not, it is copied as-is.
 *
 * \warning \p src and \p dest are expected to be the correct sizes. It is up
 *          to the caller to make sure.
 * \warning \p src and \p dest must not point to the same location
 */
template<typename T>
void spherical_transform(const BasisSet & bs,
                        const SphericalTransformMap & smap,
                        T const * const RESTRICT src,
                        T * const RESTRICT dest, size_t width)
{
    size_t srcpos = 0;
    size_t destpos = 0;
    std::fill(dest, dest + width * bs.n_functions(), static_cast<T>(0));

    for(const auto & shell : bs)
    {
        const ShellType type = shell.get_type();

        if(type == ShellType::SphericalGaussian)
        {
            const int am = shell.am();
            const int ncart = n_cartesian_gaussian(am);
            const int nsph = n_spherical_gaussian(am);

            if(!smap.count(am))
                throw BasisSetException("AM not available in the spherical transform map", "am", am);

            spherical_transform_block(smap.at(am), src + srcpos, dest + destpos, width);

            srcpos += ncart * width;
            destpos += nsph * width;
        }
        else
        {
            const size_t len = width * shell.n_functions();
            std::copy(src + srcpos, src + srcpos + len, dest + destpos);
            srcpos += len;
            destpos += len;
        }
    }
}

} // close namespace pulsar


#endif
