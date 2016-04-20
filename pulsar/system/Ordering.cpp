/*! \file
 *
 * \brief Functions related to basis function ordering (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include <cstdlib>
#include <algorithm>
#include "pulsar/system/Ordering.hpp"
#include "pulsar/system/BasisSet.hpp"
#include "pulsar/exception/Exceptions.hpp"

using namespace pulsar::exception;

namespace pulsar{
namespace system {


namespace lut {
    extern const CartesianOrderingMap ao_cartesian_order_;
    extern const SphericalOrderingMap ao_spherical_order_;
}



const SphericalOrderingMap & AllSphericalOrderings(void)
{
    return lut::ao_spherical_order_;
}


const CartesianOrderingMap & AllCartesianOrderings(void)
{
    return lut::ao_cartesian_order_;
}



const std::vector<int> & SphericalOrdering(int am)
{
    if(!lut::ao_spherical_order_.count(am))
        throw GeneralException("Angular momentum out of range", "am", am);

    return lut::ao_spherical_order_.at(am);
}



const std::vector<IJK> & CartesianOrdering(int am)
{
    if(!lut::ao_cartesian_order_.count(am))
        throw GeneralException("Angular momentum out of range", "am", am);

    return lut::ao_cartesian_order_.at(am);
}


size_t CartesianIndex(int am, IJK ijk)
{
    const auto & ijkvec = CartesianOrdering(am);
    const auto it = std::find(ijkvec.begin(), ijkvec.end(), ijk);
    if(it == ijkvec.end())
        throw GeneralException("Value of IJK not found for this am", "am", am,
                               "i", ijk[0], "j", ijk[1], "k", ijk[2]);
    else
        return std::distance(ijkvec.begin(), it);
}


size_t SphericalIndex(int am, int m)
{
    const auto & svec = SphericalOrdering(am);
    const auto it = std::find(svec.begin(), svec.end(), m);
    if(it == svec.end())
        throw GeneralException("Value of m not found for this am", "am", am,
                               "m", m);
    else
        return std::distance(svec.begin(), it);
}



// common instantiations of reorder code
#define DECLARE_TEMPLATE_REORDER(type) \
    template void ReorderAOBasis<type>(const BasisSet &, \
                                       const CartesianOrderingMap &, const CartesianOrderingMap &, \
                                       const SphericalOrderingMap &, const SphericalOrderingMap &, \
                                       type const *, type *, size_t); \
    template void ReorderAOBasis<type>(const BasisSet &, \
                                       const CartesianOrderingMap &, const CartesianOrderingMap &, \
                                       const SphericalOrderingMap &, const SphericalOrderingMap &, \
                                       std::vector<type> &, size_t); \
    template std::vector<type> ReorderAOBasis_Copy<type>(const BasisSet &, \
                                                         const CartesianOrderingMap &, const CartesianOrderingMap &, \
                                                         const SphericalOrderingMap &, const SphericalOrderingMap &, \
                                                         const std::vector<type> &, size_t); 

DECLARE_TEMPLATE_REORDER(int)
DECLARE_TEMPLATE_REORDER(double)


#undef DECLARE_TEMPLATE_REORDER



} // close namespace system
} // close namespace pulsar

