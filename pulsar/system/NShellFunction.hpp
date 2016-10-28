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


//////////////////////////////
// Cartesian
//////////////////////////////
inline size_t n_cartesian_gaussian_in_shell(const BasisShellBase & bs)
{
    const int am = bs.am();
    size_t cart = n_cartesian_gaussian(am);


    // if am < 0, the general contraction is included
    // in the n_cartesian_gaussian

    if(am < 0)
        return cart;
    else
        return cart * bs.n_general_contractions();
}

inline size_t n_cartesian_gaussian_for_shell_am(const BasisShellBase & bs)
{
    return n_cartesian_gaussian(bs.am());
}


//////////////////////////////
// Spherical Harmonics
//////////////////////////////
inline size_t n_spherical_gaussian_shell(const BasisShellBase & bs)
{
    const int am = bs.am();
    size_t sph = n_spherical_gaussian(am);


    // if am < 0, the general contraction is included
    // in the n_spherical_gaussian

    if(am < 0)
        return sph;
    else
        return sph * bs.n_general_contractions();
}


inline size_t n_spherical_gaussian_for_shell_am(const BasisShellBase & bs)
{
    return n_spherical_gaussian(bs.am());
}

} // close namespace pulsar



#endif
