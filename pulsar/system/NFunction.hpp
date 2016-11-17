/*! \file
 *
 * \brief Calculation of sperical and cartesian basis functions
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_SYSTEM__NFUNCTION_HPP_
#define PULSAR_GUARD_SYSTEM__NFUNCTION_HPP_

#include <array>
#include <algorithm>

#include "pulsar/system/ShellType.hpp"


#define n_cartesian_gaussian_(am) ((am>=0)?((((am)+2)*((am)+1))>>1):0)
#define n_spherical_gaussian_(am) ((2*(am)+1))


namespace pulsar{


//////////////////////////////
// Cartesian
//////////////////////////////

/*! \brief Sum of the number of cartesian gaussians in am = 0
 *         through the given am
 */
inline int n_sum_cartesian_gaussian(int am)
{
    int ncart = 0;
    for(int i = 0; i <= am; ++i)
        ncart += n_cartesian_gaussian_(i);
    return ncart;
}



/*! \brief Number of cartesian gaussians in a single AM
 */
inline int n_cartesian_gaussian(int am)
{
    if(am >= 0) // "Normal" angular momentum
        return n_cartesian_gaussian_(am);
    else // "Special" combination (sp, spd, etc)
        return n_sum_cartesian_gaussian(-am);
}




//////////////////////////////
// Spherical Harmonics
//////////////////////////////
/*! \brief Sum of the number of spherical gaussians in am = 0
 *         through the given am
 */
inline int n_sum_spherical_gaussian(int am)
{
    int nsph = 0;
    for(int i = 0; i <= am; ++i)
        nsph += n_spherical_gaussian_(i);
    return nsph;
}


/*! \brief Number of spherical gaussians in a single AM
 */
inline int n_spherical_gaussian(int am)
{
    if(am >= 0) // "Normal" angular momentum
        return n_spherical_gaussian_(am);
    else // "Special" combination (sp, spd, etc)
        return n_sum_spherical_gaussian(-am);
}





/////////////////
// Main functions
/////////////////

/*! \brief Determine the number of functions for a given angular momentum
 *
 * This takes into account combined shells (sp, spd, etc).
 *
 * \throw pulsar::PulsarException if the type is unknown
 */
inline int n_functions(ShellType type, int am)
{
    switch(type)
    {
        case ShellType::CartesianGaussian:
            return n_cartesian_gaussian(am);
        case ShellType::SphericalGaussian:
            return n_spherical_gaussian(am);
        default:
            throw PulsarException("Unknown shell type", "type", static_cast<int>(type));
    }
}


/*! \brief Determine the number of functions for a given angular momentum
 *
 * This takes into account combined shells (sp, spd, etc). Variation for a
 * container of angular momenta (std::array or std::vector) for which
 * all the angular momenta are multiplied together.
 *
 * \throw pulsar::PulsarException if the type is unknown
 */
template<typename Container>
int n_functions(ShellType type, const Container & amcont)
{
    return std::accumulate(amcont.begin(), amcont.end(), static_cast<int>(0),
                          [type](int sum, int am)
                          { return sum *= n_functions(type, am); });
}

} // close namespace pulsar



#endif
