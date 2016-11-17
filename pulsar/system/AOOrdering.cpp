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
#include "pulsar/exception/PulsarException.hpp"



namespace pulsar{

namespace lut {
    extern const AOOrderingMaps ao_ordering_;
}



const AOOrderingMaps & all_ao_orderings(void) noexcept
{
    return lut::ao_ordering_;
}


const std::vector<int8_t> & spherical_ordering(int am)
{
    const auto & order = all_ao_orderings().spherical_order;

    if(!order.count(am))
        throw PulsarException("Angular momentum out of range", "am", am);

    return order.at(am);
}



const std::vector<IJK> & cartesian_ordering(int am)
{
    const auto & order = all_ao_orderings().cartesian_order;

    if(!order.count(am))
        throw PulsarException("Angular momentum out of range", "am", am);

    return order.at(am);
}


size_t cartesian_index(int am, IJK ijk)
{
    const auto & ijkvec = cartesian_ordering(am);
    const auto it = std::find(ijkvec.begin(), ijkvec.end(), ijk);
    if(it == ijkvec.end())
        throw PulsarException("Value of IJK not found for this am", "am", am,
                               "i", ijk[0], "j", ijk[1], "k", ijk[2]);
    else
        return std::distance(ijkvec.begin(), it);
}


size_t full_cartesian_index(int am, IJK ijk)
{
    // we could just use the ordering for negative am, but
    // that doesn't necessarily go to the highest AM

    size_t idx = cartesian_index(am, ijk);
    if(idx <= 0)  // "negative" am would already include the lower AM
        return idx;
    else
        return idx + n_cartesian_gaussian(am-1);
}

size_t full_spherical_index(int am, int m)
{
    // we could just use the ordering for negative am, but
    // that doesn't necessarily go to the highest AM

    size_t idx = spherical_index(am, m);
    if(idx <= 0)  // "negative" am would already include the lower AM
        return idx;
    else
        return idx + n_spherical_gaussian(am-1);
}


size_t spherical_index(int am, int m)
{
    const auto & svec = spherical_ordering(am);
    const auto it = std::find(svec.begin(), svec.end(), m);
    if(it == svec.end())
        throw PulsarException("Value of m not found for this am", "am", am,
                               "m", m);
    else
        return std::distance(svec.begin(), it);
}



BSReorderMap make_basis_reorder_map(const AOOrderingMaps & src,
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
            ret[ShellType::SphericalGaussian][it.first] = make_ordering(srcord, it.second);
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
            ret[ShellType::CartesianGaussian][it.first] = make_ordering(srcord, it.second);
        }
    }

    return ret;
}



std::vector<size_t> make_ao_basis_ordering(const BasisSet & bs, const BSReorderMap & bm)
{
    
    
    // creat the full reorder vector
    std::vector<size_t> fullreorder;
    fullreorder.reserve(bs.n_functions());

    size_t offset = 0;
    
    for(const auto & shell : bs)
    {   
        // get the reordering for this shell
        ShellType type = shell.get_type();
        int am = shell.am();

        if(bm.count(type) == 0)
            throw PulsarException("Missing shell type in BSReorderMap",
                                    "type", ShellTypeString(type));

        if(bm.at(type).count(am) == 0)
            throw PulsarException("Missing AM in BSReorderMap for this shell type",
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
    if(fullreorder.size() != bs.n_functions())
        throw PulsarException("Basis set reordering inconsistency. size is incorrect",
                                "nfunc", bs.n_functions(), "fullreorder", fullreorder.size());
    
    return fullreorder;
}

} // close namespace pulsar

