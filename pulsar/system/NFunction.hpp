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


#define NCartesianGaussian_(am) ((am>=0)?((((am)+2)*((am)+1))>>1):0)
#define NSphericalGaussian_(am) ((2*(am)+1))


namespace pulsar{
namespace system {


//////////////////////////////
// Cartesian
//////////////////////////////
inline int NCartesianGaussian(int am)
{
    if(am >= 0) // "Normal" angular momentum
        return NCartesianGaussian_(am);
    else // "Special" combination (sp, spd, etc)
    {
        int ncart = 0;
        for(int i = 0; i >= am; --i)
            ncart += NCartesianGaussian_(-i);
        return ncart;
    }
}



//////////////////////////////
// Spherical Harmonics
//////////////////////////////
inline int NSphericalGaussian(int am)
{
    if(am >= 0) // "Normal" angular momentum
        return NSphericalGaussian_(am);
    else // "Special" combination (sp, spd, etc)
    {
        int ncart = 0;
        for(int i = 0; i >= am; --i)
            ncart += NSphericalGaussian_(-i);
        return ncart;
    }
}





/////////////////
// Main functions
/////////////////

/*! \brief Determine the number of functions for a given angular momentum
 *
 * This takes into account combined shells (sp, spd, etc).
 *
 * \throw pulsar::exception::BasisSetException if the type is unknown
 */
inline int NFunction(ShellType type, int am)
{
    switch(type)
    {
        case ShellType::CartesianGaussian:
            return NCartesianGaussian(am);
        case ShellType::SphericalGaussian:
            return NSphericalGaussian(am);
        default:
            throw exception::BasisSetException("Unknown shell type", "type", static_cast<int>(type));
    }
}


/*! \brief Determine the number of functions for a given angular momentum
 *
 * This takes into account combined shells (sp, spd, etc). Variation for a
 * container of angular momenta (std::array or std::vector) for which
 * all the angular momenta are multiplied together.
 *
 * \throw pulsar::exception::BasisSetException if the type is unknown
 */
template<typename Container>
int NFunction(ShellType type, const Container & amcont)
{
    return std::accumulate(amcont.begin(), amcont.end(), static_cast<int>(0),
                          [type](int sum, int am)
                          { return sum *= NFunction(type, am); });
}


} // close namespace system
} // close namespace pulsar



#endif
