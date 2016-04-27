/*! \file
 *
 * \brief Calculation of sperical and cartesian basis functions
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_SYSTEM__NSHELLFUNCTION_HPP_
#define PULSAR_GUARD_SYSTEM__NSHELLFUNCTION_HPP_


#include "pulsar/system/NFunction.hpp"
#include "pulsar/system/BasisShellBase.hpp"


namespace pulsar{
namespace system {


//////////////////////////////
// Cartesian
//////////////////////////////
inline size_t NCartesianGaussianInShell(const BasisShellBase & bs)
{
    const int am = bs.AM();
    size_t cart = NCartesianGaussian(am);


    // if am < 0, the general contraction is included
    // in the NCartesianGaussian

    if(am < 0)
        return cart;
    else
        return cart * bs.NGeneral();
}

inline size_t NCartesianGaussianForShellAM(const BasisShellBase & bs)
{
    return NCartesianGaussian(bs.AM());
}


//////////////////////////////
// Spherical Harmonics
//////////////////////////////
inline size_t NSphericalGaussianShell(const BasisShellBase & bs)
{
    const int am = bs.AM();
    size_t sph = NSphericalGaussian(am);


    // if am < 0, the general contraction is included
    // in the NSphericalGaussian

    if(am < 0)
        return sph;
    else
        return sph * bs.NGeneral();
}


inline size_t NSphericalGaussianForShellAM(const BasisShellBase & bs)
{
    return NSphericalGaussian(bs.AM());
}


} // close namespace system
} // close namespace pulsar



#endif
