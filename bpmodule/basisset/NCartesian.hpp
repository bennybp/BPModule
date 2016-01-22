#ifndef BPMODULE_GUARD_BASISSET__NCARTESIAN_HPP_
#define BPMODULE_GUARD_BASISSET__NCARTESIAN_HPP_

#include <array>
#include <algorithm>

#define NCARTESIAN_(am) ((am>=0)?((((am)+2)*((am)+1))>>1):0)
#define NSPHERICAL_(am) ((2*(am)+1))


//////////////////////////////
// Cartesian
//////////////////////////////
inline int NCARTESIAN(int am)
{
    return NCARTESIAN_(am);
}


template<size_t N>
int NCARTESIAN(const std::array<int, N> & v)
{
    return std::accumulate(v.begin(), v.end(), 0,
                          [](int i, int j){ return i *= NCARTESIAN_(j); });
}




//////////////////////////////
// Spherical Harmonics
//////////////////////////////
inline int NSPHERICAL(int am)
{
    return NSPHERICAL_(am);
}


template<size_t N>
int NSPHERICAL(const std::array<int, N> & v)
{
    return std::accumulate(v.begin(), v.end(), 0,
                          [](int i, int j){ return i *= NSPHERICAL_(j); });
}


#endif
