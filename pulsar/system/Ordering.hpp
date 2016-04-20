/*! \file
 *
 * \brief Functions related to basis function ordering (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef BPMODULE_GUARD_SYSTEM__ORDERING_HPP_
#define BPMODULE_GUARD_SYSTEM__ORDERING_HPP_

#include <vector>
#include <map>
#include "bpmodule/system/BasisSet.hpp"


namespace bpmodule {
namespace system {


typedef std::array<int, 3> IJK; // exponents on x, y, z of a cartesian gaussian

typedef std::map<int, std::vector<int>> SphericalOrderingMap;
typedef std::map<int, std::vector<IJK>> CartesianOrderingMap;




namespace detail {

/////////////////////////
// Helper functions
/////////////////////////
/*! \brief Reorders a block based on a new ordering
 *
 *
 * The vector \p neworder contains indices of the original buffer,
 * relative to the beginning of that buffer (zero-based).
 *
 * Ie, buf = ABCDEFG, neworder = { 3, 5, 1, 2, 4, 6, 0 }
 * results in DFBCEGA
 *
 * The length represents the number of elements for each entry. For example,
 * rows of a matrix can be reordered by setting length to the number of columns.
 *
 *
 * \param [in] neworder Ordering to change to
 * \param [in] buf Buffer to change
 * \param [in] length The number of elements each entry contains.
 * \param [in] src A buffer containing the source to copy. Should not be aliased to \p buf
 *
 * \note \p src must be at least length * neworder.size() in size.
 *          \p buf is expected to be that size as well
 *
 */
template<typename T>
static void ReorderBlock(const std::vector<size_t> & neworder,
                                T * buf, size_t length, T * src)
{
    size_t size = neworder.size();

    for(size_t i = 0; i < size; i++)
    {
        size_t idx = neworder[i];
        std::copy(src + idx*length,
                  src + idx*length + length,
                  buf + i*length);
    }
}



template<typename T>
static std::vector<size_t> MakeOrdering(const std::vector<T> & src, const std::vector<T> & dest)
{
    using exception::GeneralException;

    std::vector<size_t> ret;
    ret.resize(src.size());

    if(src.size() != dest.size())
       throw GeneralException("Inconsistent sizes for reordering vectors",
                              "fromsize", src.size(), "tosize", dest.size());

    for(const auto & it : src)
    {
        const auto to_pos = std::find(dest.begin(), dest.end(), it);
        if(to_pos == dest.end())
            throw GeneralException("Cannot find element in destination vector");

        ret.push_back(std::distance(dest.begin(), to_pos));
    }

    return ret;
}
///////////////////////////////
// End Helper Functions
///////////////////////////////

} // close namespace detail



/*! \brief Obtain the ordering of spherical harmonics for all available angular momenta */
const SphericalOrderingMap & AllSphericalOrderings(void);


/*! \brief Obtain the ordering of spherical harmonics for all available angular momenta */
const CartesianOrderingMap & AllCartesianOrderings(void);


/*! \brief Obtain the ordering of m_l for a given angular momentum
 *
 * \throw bpmodule::exception::GeneralException if the value of the
 *        angular momentum is out of range
 */
const std::vector<int> & SphericalOrdering(int am);


/*! \brief Obtain the ordering of cartesian gaussians for a given angular momentum
 *
 * \throw bpmodule::exception::GeneralException if the value of the
 *        angular momentum is out of range
 */
const std::vector<IJK> & CartesianOrdering(int am);


/*! \brief Get the index of a spherical basis function in the usual ordering
 *
 * \throw bpmodule::exception::GeneralException if the value of the
 *        angular momentum is out of range or if the value of ijk
 *        is not found for the given am
 *
 * \param [in] am The angular momentum to search for
 * \param [in] ijk Exponents on x, y, and z of the cartesian gaussian
 */
size_t CartesianIndex(int am, IJK ijk);


/*! \brief Get the index of a spherical basis function in the usual ordering
 *
 * \throw bpmodule::exception::GeneralException if the value of the
 *        angular momentum is out of range or if the value of ijk
 *        is not found for the given am
 *
 * \param [in] am The angular momentum to search for
 * \param [in] m The value of m_l of the spherical harmonic to search for
 */
size_t SphericalIndex(int am, int m);




/*! \brief Reorder a buffer based on the AO ordering
 *
 * The length represents the number of elements for each entry. For example,
 * rows of a matrix can be reordered by setting length to the number of columns.
 *
 * \param [in] bs The basis set that the data relates to
 * \param [in] cartmapfrom A map containing the source cartesian orderings
 * \param [in] cartmapto A map containing the desired cartesian orderings
 * \param [in] sphmapfrom A map containing the source spherical harmonic orderings
 * \param [in] sphmapto A map containing the desired spherical harmonic orderings
 * \param [in] src Source of data to reorder
 * \param [in] dest Where to put the reordered data
 * \param [in] length The number of elements each entry contains.
 *
 *
 * \note \p src and \p dest may point to the same data
 *
 * \note \p src and \p dest is expected to be bs.NFunctions() * length in size
 *
 * \throw bpmodule::exception::GeneralException if there is an inconsistency (angular
 *        momentum out of range, and index out of range in the orderings, etc)
 */
template<typename T>
void ReorderAOBasis(const BasisSet & bs,
                    const CartesianOrderingMap & cartmapfrom,
                    const CartesianOrderingMap & cartmapto,
                    const SphericalOrderingMap & sphmapfrom,
                    const SphericalOrderingMap & sphmapto,
                    T const * src, T * dest, size_t length)
{
    using exception::GeneralException;

    //! \todo skip some steps if basis set doesn't have cartesian or spherical
    typedef std::map<int, std::vector<size_t>> OrderChangeMap;


    std::set<int> allam = bs.AllAM();

    OrderChangeMap cart_om, sph_om;

    for(auto am : allam)
    {
        if(!cartmapfrom.count(am))
            throw GeneralException("Angular momentum out of range of cartmapfrom", "am", am);
        if(!cartmapto.count(am))
            throw GeneralException("Angular momentum out of range of cartmapto", "am", am);
        if(!sphmapfrom.count(am))
            throw GeneralException("Angular momentum out of range of sphmapfrom", "am", am);
        if(!sphmapto.count(am))
            throw GeneralException("Angular momentum out of range of sphmapto", "am", am);


        const auto & cartfrom_vec = cartmapfrom.at(am);
        const auto & cartto_vec = cartmapto.at(am);
        const auto & sphfrom_vec = sphmapfrom.at(am);
        const auto & sphto_vec = sphmapto.at(am);

        if(cartfrom_vec.size() != cartto_vec.size())
            throw GeneralException("Inconsistent sizes for cartfrom_vec and cartto_vec", "am", am,
                                   "fromsize", cartfrom_vec.size(), "tosize", cartto_vec.size());

        if(sphfrom_vec.size() != sphto_vec.size())
            throw GeneralException("Inconsistent sizes for sphfrom_vec and sphto_vec", "am", am,
                                   "fromsize", sphfrom_vec.size(), "tosize", sphto_vec.size());

        std::vector<size_t> cart_ordvec = detail::MakeOrdering(cartfrom_vec, cartto_vec);
        std::vector<size_t> sph_ordvec = detail::MakeOrdering(sphfrom_vec, sphto_vec);
        cart_om.emplace(am, std::move(cart_ordvec));
        sph_om.emplace(am, std::move(sph_ordvec));
    }


    // make a temporary worksapce
    std::vector<T> tmp(bs.MaxNFunctions() * length);

    size_t pos = 0;

    for(const auto & shell : bs)
    {
        size_t blocksize = shell.NFunctions()*length;
        std::copy(src + pos,
                  src + pos + blocksize,
                  tmp.data());

        if(shell.GetType() == ShellType::SphericalGaussian)
            detail::ReorderBlock(sph_om[shell.AM()], dest + pos, length, tmp.data());
        else if(shell.GetType() == ShellType::CartesianGaussian)
            detail::ReorderBlock(cart_om[shell.AM()], dest + pos, length, tmp.data());
        else
            throw GeneralException("Type not implemented");

        pos += blocksize;
    }
}


/*! In-place Reorder a vector of data based on AO ordering
 */
template<typename T>
void ReorderAOBasis(const BasisSet & bs,
                    const CartesianOrderingMap & cartmapfrom,
                    const CartesianOrderingMap & cartmapto,
                    const SphericalOrderingMap & sphmapfrom,
                    const SphericalOrderingMap & sphmapto,
                    std::vector<T> & src, size_t length)
{
    using exception::GeneralException;

    if(src.size() != length * bs.NFunctions())
        throw GeneralException("Bad size of vector passed to ReorderAOBasis", "size", src.size(),
                               "expected", length * bs.NFunctions());

    ReorderAOBasis(bs, cartmapfrom, cartmapto,
                   sphmapfrom, sphmapto,
                   src.data(), src.data(), length);
}


/*! Reorder a vector of data based on AO ordering, returning a new vector
 */
template<typename T>
std::vector<T>
ReorderAOBasis_Copy(const BasisSet & bs,
                    const CartesianOrderingMap & cartmapfrom,
                    const CartesianOrderingMap & cartmapto,
                    const SphericalOrderingMap & sphmapfrom,
                    const SphericalOrderingMap & sphmapto,
                    const std::vector<T> & src, size_t length)
{
    using exception::GeneralException;

    if(src.size() != length * bs.NFunctions())
        throw GeneralException("Bad size of vector passed to ReorderAOBasis", "size", src.size(),
                               "expected", length * bs.NFunctions());

    std::vector<T> ret(src.size());
    ReorderAOBasis(bs, cartmapfrom, cartmapto,
                   sphmapfrom, sphmapto,
                   src.data(), ret.data(), length);
    return ret;
}



/*! Reorder a vector of data based on cartesian AO ordering, returning a new vector
 */
template<typename T>
std::vector<T>
ReorderCartesianAOBasis_Copy(const BasisSet & bs,
                             const CartesianOrderingMap & cartmapfrom,
                             const CartesianOrderingMap & cartmapto,
                             const std::vector<T> & src, size_t length)
{
    return ReorderAOBasis_Copy(bs, cartmapfrom, cartmapto,
                               SphericalOrderingMap(), SphericalOrderingMap(),
                               src, length); 
}

/*! Reorder a vector of data based on cartesian AO ordering, returning a new vector
 */
template<typename T>
std::vector<T>
ReorderSphericalAOBasis_Copy(const BasisSet & bs,
                             const SphericalOrderingMap & sphmapfrom,
                             const SphericalOrderingMap & sphmapto,
                             const std::vector<T> & src, size_t length)
{
    return ReorderAOBasis_Copy(bs, CartesianOrderingMap(), CartesianOrderingMap(),
                               sphmapfrom, sphmapto, 
                               src, length); 
}

// common instantiations
#define DECLARE_EXTERN_TEMPLATE_REORDER(type) \
    extern template void ReorderAOBasis<type>(const BasisSet &, \
                                              const CartesianOrderingMap &, const CartesianOrderingMap &, \
                                              const SphericalOrderingMap &, const SphericalOrderingMap &, \
                                              type const *, type *, size_t); \
    extern template void ReorderAOBasis<type>(const BasisSet &, \
                                              const CartesianOrderingMap &, const CartesianOrderingMap &, \
                                              const SphericalOrderingMap &, const SphericalOrderingMap &, \
                                              std::vector<type> &, size_t); \
    extern template std::vector<type> ReorderAOBasis_Copy<type>(const BasisSet &, \
                                                                const CartesianOrderingMap &, const CartesianOrderingMap &, \
                                                                const SphericalOrderingMap &, const SphericalOrderingMap &, \
                                                                const std::vector<type> &, size_t);


DECLARE_EXTERN_TEMPLATE_REORDER(int)
DECLARE_EXTERN_TEMPLATE_REORDER(double)


#undef DECLARE_EXTERN_TEMPLATE_REORDER



} // close namespace system
} // close namespace bpmodule


#endif
