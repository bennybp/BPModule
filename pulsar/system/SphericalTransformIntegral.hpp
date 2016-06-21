/*! \file
 *
 * \brief Herlpers for spherical transformation of integrals
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_SYSTEM__SPHERICALTRANSFORMINTEGRAL_HPP_
#define PULSAR_GUARD_SYSTEM__SPHERICALTRANSFORMINTEGRAL_HPP_


#include "pulsar/system/SphericalTransform.hpp"
#include "pulsar/system/NShellFunction.hpp"
#include "pulsar/system/GeneralShellIterator.hpp"

namespace pulsar{
namespace system {

/* transformation of a generic batch of integrals
 *
 * The integrals should all be in cartesian form.
 * Whether each is transformed depends on the type
 * stored in the shells in the array.
 *
 * \p source size should be the number of cartesian integrals
 * (taking into account general contractions). Work should also be
 * that size.
 *
 * \p dest should be big enough to hold the transformed integrals.
 * This depends on if each shell is spherical or not. For simplicity,
 * this could be as big as \p source.
 *
 * Cartesian ordering is expected to be pulsar ordering.
 */
template<int N>
void CartesianToSpherical(std::array<std::reference_wrapper<const system::BasisShellBase>, N> shells,
                          double * source, double * dest, double * work, int ncomp)
{
    using namespace system;

    std::array<bool, N> isspherical;
    size_t allncart = 1;  // Total number of cartesians in this container
    int nsphshells = 0;   // How many of the BasisShellBases are spherical

    for(int i = 0; i < N; i++)
    {
        const system::BasisShellBase & sh = shells[i].get();
        const bool issph = isspherical[i] = (sh.get_type() == ShellType::SphericalGaussian);

        isspherical[i] = issph;
        allncart *= n_cartesian_gaussian_in_shell(shells[i]);

        if(issph)
            nsphshells++;
    }


    if(nsphshells == 0) // nothing to do
    {
        std::copy(source, source + ncomp*allncart, dest);
        return;
    }


    // loop over the number of specified components
    for(int c = 0; c < ncomp; c++)
    {
        // iterator over the general contractions
        // We have the transformations for combined AM shells, so
        // no need to split them in the iterator.
        system::GeneralShellIterator<N> gsi(shells, false);

        // this is equivalent to an N-level for loop over the combinations of
        // the general contractions
        do {
            double * buf1 = source;
            double * buf2 = work;

            std::array<int, N> gam;  // am for this general contraction combination
            std::array<size_t, N> gncart; // number of cartesians for this general contraction combination
            std::array<size_t, N> gnfunc; // number of functions for this general contraction combination

            // store how much to advance the source and destination pointers
            size_t source_adv = 1;
            size_t dest_adv = 1;

            for(int i = 0; i < N; i++)
            {
                const size_t genidx = gsi.GeneralIdx(i);
                gam[i] = gsi.AM(i);
                gncart[i] = n_cartesian_gaussian(gam[i]);
                gnfunc[i] = shells[i].get().general_n_functions(genidx);

                source_adv *= gncart[i]; // source holds only cartesian
                dest_adv *= gnfunc[i];
            }


            // the starting width is the number of cartesian functions for
            // the last (N-1) shells. However, we divide first inside the loop
            size_t width = 1;
            for(int i = 0; i < N; i++)
                width *= gncart[i];

            // the starting number of iterations is 1, of course
            size_t niter = 1;

            // loop over the N shells and transform if we have to
            for(int i = 0; i < N; i++)
            {
                width /= gncart[i];

                if(isspherical[i])
                {
                    const auto & coef = spherical_transform_for_am(gam[i]);

                    spherical_transform_block(coef, buf1, buf2, width, gam[i], niter);

                    // swap the source and destination
                    std::swap(buf1, buf2);
                }

                niter *= gnfunc[i];
            }

            // buf1 always contains the final, transformed, intergals since we swap at
            // the end of the above loop. It may point to either source or work, though.

            // We have to copy to the destination
            // buf1 will always contain the final transformed integrals (since we always swap at the
            // end of the loop).
            // The amount we did is equivalent to how much we have to advance the buffer pointers

            // dest_adv is also the number of final transformed integrals we did
            std::copy(buf1, buf1 + dest_adv, dest);

            source += source_adv;
            dest += dest_adv;

        } while(gsi.Next());
    }
}


/*! \brief transforms two-center integrals over gaussian basis functions
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
inline
void CartesianToSpherical_2Center(const system::BasisShellBase & sh1,
                                  const system::BasisShellBase & sh2,
                                  double * source, double * dest, double * work,
                                  int ncomp)
{
    CartesianToSpherical<2>({{sh1, sh2}}, source, dest, work, ncomp);
}

inline
void CartesianToSpherical_3Center(const system::BasisShellBase & sh1,
                                  const system::BasisShellBase & sh2,
                                  const system::BasisShellBase & sh3,
                                  double * source, double * dest, double * work,
                                  int ncomp)
{
    CartesianToSpherical<3>({{sh1, sh2, sh3}}, source, dest, work, ncomp);
}


inline
void CartesianToSpherical_4Center(const system::BasisShellBase & sh1,
                                  const system::BasisShellBase & sh2,
                                  const system::BasisShellBase & sh3,
                                  const system::BasisShellBase & sh4,
                                  double * source, double * dest, double * work,
                                  int ncomp)
{
    CartesianToSpherical<4>({{sh1, sh2, sh3, sh4}}, source, dest, work, ncomp);
}


} // close namespace system
} // close namespace pulsar


#endif
