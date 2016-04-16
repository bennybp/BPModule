/*! \file
 *
 * \brief Calculation of sperical and cartesian basis functions
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef BPMODULE_GUARD_SYSTEM__NFUNCTION_HPP_
#define BPMODULE_GUARD_SYSTEM__NFUNCTION_HPP_

#include <array>
#include <algorithm>

#define NCARTESIAN_(am) ((am>=0)?((((am)+2)*((am)+1))>>1):0)
#define NSPHERICAL_(am) ((2*(am)+1))


//////////////////////////////
// Cartesian
//////////////////////////////
inline int NCARTESIAN(int am)
{
    if(am >= 0) // "Normal" angular momentum
        return NCARTESIAN_(am);
    else // "Special" combination (sp, spd, etc)
    {
        int ncart = 0;
        for(int i = 0; i >= am; --i)
            ncart += NCARTESIAN_(-i);
        return ncart;
    }
}


template<size_t N>
int NCARTESIAN(const std::array<int, N> & v)
{
    return std::accumulate(v.begin(), v.end(), static_cast<int>(0),
                          [](int i, int j)
                          { return i *= NCARTESIAN_(j); });
}




//////////////////////////////
// Spherical Harmonics
//////////////////////////////
inline int NSPHERICAL(int am)
{
    if(am >= 0) // "Normal" angular momentum
        return NSPHERICAL_(am);
    else // "Special" combination (sp, spd, etc)
    {
        int ncart = 0;
        for(int i = 0; i >= am; --i)
            ncart += NSPHERICAL_(-i);
        return ncart;
    }
}


template<size_t N>
int NSPHERICAL(const std::array<int, N> & v)
{
    return std::accumulate(v.begin(), v.end(), static_cast<int>(0),
                          [](int i, int j)
                          { return i *= NSPHERICAL_(j); });
}


#endif
