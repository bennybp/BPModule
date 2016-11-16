/*! \file
 *
 * \brief Functions related to basis function ordering (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_SYSTEM__AOORDERING_HPP_
#define PULSAR_GUARD_SYSTEM__AOORDERING_HPP_

#include <map>
#include <cstdint>
#include "pulsar/math/Reorder.hpp"
#include "pulsar/system/ShellType.hpp"


namespace pulsar{

// forward declare
class BasisSet;


/// Exponents on the x, y, z prefactors of a cartesian gaussian
typedef std::array<uint8_t, 3> IJK;

/// Maps angular momentum to a vector of m_l values in the desired order
typedef std::map<int, std::vector<int8_t>> SphericalAOOrderingMap;

/// Maps angular momentum to a vector of IJK in the desired order
typedef std::map<int, std::vector<IJK>> CartesianAOOrderingMap;


/*! \brief Ordering of all types of basis functions */
struct AOOrderingMaps
{
    SphericalAOOrderingMap spherical_order;
    CartesianAOOrderingMap cartesian_order;
};


//! A map of angular momentum to new ordering vectors (see MakeOrdering)
typedef std::map<int, std::vector<size_t>> AMReorderMap;

//! A map for changing one AO ordering to another for all shell types
typedef std::map<ShellType, AMReorderMap> BSReorderMap;



/*! \brief Obtain the ordering in pulsar for all
 *         available angular momenta */
const AOOrderingMaps & all_ao_orderings(void) noexcept;


/*! \brief Obtain the ordering in pulsar of m_l for gaussians of
 *         a given angular momentum
 *
 * \throw pulsar::BasisSetException if the value of the
 *        angular momentum is out of range
 */
const std::vector<int8_t> & spherical_ordering(int am);


/*! \brief Obtain the ordering in pulsar of cartesian gaussians for
 *         a given angular momentum
 *
 * \throw pulsar::BasisSetException if the value of the
 *        angular momentum is out of range
 */
const std::vector<IJK> & cartesian_ordering(int am);


/*! \brief Get the index of a cartesian gaussian basis function
 *         in pulsar ordering
 *
 * \throw pulsar::BasisSetException if the value of the
 *        angular momentum is out of range or if the value of ijk
 *        is not found for the given am
 *
 * \param [in] am The angular momentum to search for
 * \param [in] ijk Exponents on x, y, and z of the cartesian gaussian
 */
size_t cartesian_index(int am, IJK ijk);


/*! \brief Get the index of a cartesian gaussian basis function
 *         among functions for [0, \p am] in pulsar ordering
 *
 * \throw pulsar::BasisSetException if the value of the
 *        angular momentum is out of range or if the value of ijk
 *        is not found for the given am
 *
 * \param [in] am The angular momentum to search for
 * \param [in] ijk Exponents on x, y, and z of the cartesian gaussian
 */
size_t full_cartesian_index(int am, IJK ijk);


/*! \brief Get the index of a spherical gaussian basis function
 *         in pulsar ordering
 *
 * \throw pulsar::BasisSetException if the value of the
 *        angular momentum is out of range or if the value of m
 *        is not found for the given am
 *
 * \param [in] am The angular momentum to search for
 * \param [in] m The value of m_l of the spherical harmonic to search for
 */
size_t spherical_index(int am, int m);


/*! \brief Get the index of a spherical gaussian basis function
 *         among functions for [0, \p am] in pulsar ordering
 *
 * \throw pulsar::BasisSetException if the value of the
 *        angular momentum is out of range or if the value of m
 *        is not found for the given am
 *
 * \param [in] am The angular momentum to search for
 * \param [in] m The value of m_l of the spherical harmonic to search for
 */
size_t full_spherical_index(int am, int m);



/*! \brief Creates a BSReorderMap from two AO ordering structures
 *
 * Return type represents the transformation that, if applied
 * to something in \p src ordering, will result in the
 * \p dest ordering.
 *
 * If data for an angular momentum exists in \p dest but not
 * in \p src, it is skipped.
 *
 * \throw pulsar::MathException if an element
 *        in \p dest does not exist in \p src
 */ 
BSReorderMap make_basis_reorder_map(const AOOrderingMaps & src,
                                    const AOOrderingMaps & dest);



/*! \brief Make the mapping of all AOs from one basis
 *         set ordering to another
 *
 * The \p bm argument contains how various types of shells must be
 * reordered. This is then applied via the shells contained in the
 * \p bs BasisSet. The result is a complete mapping of AO from
 * one basis set ordering to another.
 *
 * If this were applied to a vector or array of bs.n_functions()
 * elements, this would reorder the entire vector or array to the
 * new ordering.
 *
 * \throw pulsar::BasisSetException if ordering data is
 *        missing for a given angular momentum and/or shell type.
 */
std::vector<size_t> make_ao_basis_ordering(const BasisSet & bs, const BSReorderMap & bm);

} // close namespace pulsar


#endif
