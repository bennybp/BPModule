/*! \file
 *
 * \brief Functions related to basis function ordering (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_SYSTEM__AOORDERING_HPP_
#define PULSAR_GUARD_SYSTEM__AOORDERING_HPP_

#include <map>
#include "pulsar/math/Reorder.hpp"
#include "pulsar/system/ShellType.hpp"


namespace pulsar{
namespace system {


// forward declare
class BasisSet;


/// Exponents on the x, y, z prefactors of a cartesian gaussian
typedef std::array<int, 3> IJK;

/// Maps angular momentum to a vector of m_l values in the desired order
typedef std::map<int, std::vector<int>> SphericalAOOrderingMap;

/// Maps angular momentum to a vector of IJK in the desired order
typedef std::map<int, std::vector<IJK>> CartesianAOOrderingMap;


/*! \brief Ordering of all types of basis functions */
struct AOOrderingMaps
{
    SphericalAOOrderingMap spherical_order;
    CartesianAOOrderingMap cartesian_order;
};


//! A map of angular momentum to new ordering vectors (see math::MakeOrdering)
typedef std::map<int, std::vector<size_t>> AMReorderMap;

//! A map for changing one AO ordering to another for all shell types
typedef std::map<ShellType, AMReorderMap> BSReorderMap;



/*! \brief Obtain the ordering in pulsar for all
 *         available angular momenta */
const AOOrderingMaps & AllAOOrderings(void) noexcept;


/*! \brief Obtain the ordering in pulsar of m_l for gaussians of
 *         a given angular momentum
 *
 * \throw pulsar::exception::BasisSetException if the value of the
 *        angular momentum is out of range
 */
const std::vector<int> & SphericalOrdering(int am);


/*! \brief Obtain the ordering in pulsar of cartesian gaussians for
 *         a given angular momentum
 *
 * \throw pulsar::exception::BasisSetException if the value of the
 *        angular momentum is out of range
 */
const std::vector<IJK> & CartesianOrdering(int am);


/*! \brief Get the index of a cartesian gaussian basis function
 *         in pulsar ordering
 *
 * \throw pulsar::exception::BasisSetException if the value of the
 *        angular momentum is out of range or if the value of ijk
 *        is not found for the given am
 *
 * \param [in] am The angular momentum to search for
 * \param [in] ijk Exponents on x, y, and z of the cartesian gaussian
 */
size_t CartesianIndex(int am, IJK ijk);


/*! \brief Get the index of a spherical gaussian basis function
 *         in pulsar ordering
 *
 * \throw pulsar::exception::BasisSetException if the value of the
 *        angular momentum is out of range or if the value of m
 *        is not found for the given am
 *
 * \param [in] am The angular momentum to search for
 * \param [in] m The value of m_l of the spherical harmonic to search for
 */
size_t SphericalIndex(int am, int m);



/*! \brief Creates a BSReorderMap from two AO ordering structures
 *
 * Return type represents the transformation that, if applied
 * to something in \p src ordering, will result in the
 * \p dest ordering.
 *
 * If data for an angular momentum exists in \p dest but not
 * in \p src, it is skipped.
 *
 * \throw pulsar::exception::MathException if an element
 *        in \p dest does not exist in \p src
 */ 
BSReorderMap MakeBSReorderMap(const AOOrderingMaps & src,
                              const AOOrderingMaps & dest);



/*! \brief Make the mapping of all AOs from one basis
 *         set ordering to another
 *
 * The \p bm argument contains how various types of shells must be
 * reordered. This is then applied via the shells contained in the
 * \p bs BasisSet. The result is a complete mapping of AO from
 * one basis set ordering to another.
 *
 * If this were applied to a vector or array of bs.NFunctions()
 * elements, this would reorder the entire vector or array to the
 * new ordering.
 *
 * \throw pulsar::exception::BasisSetException if ordering data is
 *        missing for a given angular momentum and/or shell type.
 */
std::vector<size_t> MakeAOBasisOrdering(const BasisSet & bs, const BSReorderMap & bm);




} // close namespace system
} // close namespace pulsar


#endif
