/*! \file
 *
 * \brief Functions related to basis function ordering (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include <cstdlib>
#include <algorithm>
#include "pulsar/system/AOOrdering.hpp"
#include "pulsar/system/NFunction.hpp"
#include "pulsar/system/BasisSet.hpp"
#include "pulsar/exception/Exceptions.hpp"

using namespace pulsar::exception;

namespace pulsar{
namespace system {


namespace lut {
    extern const AOOrderingMaps ao_ordering_;
}



const AOOrderingMaps & AllAOOrderings(void) noexcept
{
    return lut::ao_ordering_;
}


const std::vector<int8_t> & SphericalOrdering(int am)
{
    const auto & order = AllAOOrderings().spherical_order;

    if(!order.count(am))
        throw GeneralException("Angular momentum out of range", "am", am);

    return order.at(am);
}



const std::vector<IJK> & CartesianOrdering(int am)
{
    const auto & order = AllAOOrderings().cartesian_order;

    if(!order.count(am))
        throw GeneralException("Angular momentum out of range", "am", am);

    return order.at(am);
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


size_t FullCartesianIndex(int am, IJK ijk)
{
    // we could just use the ordering for negative am, but
    // that doesn't necessarily go to the highest AM

    size_t idx = CartesianIndex(am, ijk);
    if(idx <= 0)  // "negative" am would already include the lower AM
        return idx;
    else
        return idx + NCartesianGaussian(am-1);
}

size_t FullSphericalIndex(int am, int m)
{
    // we could just use the ordering for negative am, but
    // that doesn't necessarily go to the highest AM

    size_t idx = SphericalIndex(am, m);
    if(idx <= 0)  // "negative" am would already include the lower AM
        return idx;
    else
        return idx + NSphericalGaussian(am-1);
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



BSReorderMap MakeBSReorderMap(const AOOrderingMaps & src,
                              const AOOrderingMaps & dest)
{
    ///////////////////////////////////////
    // Consider refactoring into
    // a helper function if more are added
    ///////////////////////////////////////

    BSReorderMap ret;

    // spherical AOs
    for(const auto & it : dest.spherical_order)
    {
        // it.first is the am
        // it.second is a vector of m_l (in some specific order)
        if(src.spherical_order.count(it.first)) // if src has this AM
        {
            const auto & srcord = src.spherical_order.at(it.first);
            ret[ShellType::SphericalGaussian][it.first] = math::MakeOrdering(srcord, it.second);
        }
    }

    // cartesian AOs
    for(const auto & it : dest.cartesian_order)
    {
        // it.first is the am
        // it.second is a vector of IJK (in some specific order)
        if(src.cartesian_order.count(it.first)) // if src has this AM
        {
            const auto & srcord = src.cartesian_order.at(it.first);
            ret[ShellType::CartesianGaussian][it.first] = math::MakeOrdering(srcord, it.second);
        }
    }

    return ret;
}



std::vector<size_t> MakeAOBasisOrdering(const BasisSet & bs, const BSReorderMap & bm)
{
    using exception::BasisSetException;
    
    // creat the full reorder vector
    std::vector<size_t> fullreorder;
    fullreorder.reserve(bs.NFunctions());

    size_t offset = 0;
    
    for(const auto & shell : bs)
    {   
        // get the reordering for this shell
        ShellType type = shell.GetType();
        int am = shell.AM();

        if(bm.count(type) == 0)
            throw BasisSetException("Missing shell type in BSReorderMap",
                                    "type", ShellTypeString(type));

        if(bm.at(type).count(am) == 0)
            throw BasisSetException("Missing AM in BSReorderMap for this shell type",
                                    "type", ShellTypeString(type),
                                    "am", am);

        const auto & v = bm.at(type).at(am);
        
        // add it to the vector, keeping track of the offset
        for(auto c : v)
            fullreorder.push_back(c + offset);

        offset += v.size();
    }
    
    // full reorder should now contain the entire mapping of one
    // basis set ordering to another. So it should be of the same
    // number of functions...
    if(fullreorder.size() != bs.NFunctions())
        throw BasisSetException("Basis set reordering inconsistency. Size is incorrect",
                                "nfunc", bs.NFunctions(), "fullreorder", fullreorder.size());
    
    return fullreorder;
}


} // close namespace system
} // close namespace pulsar

