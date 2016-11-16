/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/** \file CrystalFunctions.hpp
 *  \brief A series of free functions for manipulating crystals
 *  \author Ryan M. Richard
 *  \version 1.0
 *  \date May 23, 2016
 */

#ifndef PULSAR_GUARD_CRYSTALFUNCTIONS_HPP
#define PULSAR_GUARD_CRYSTALFUNCTIONS_HPP

#include "pulsar/system/System.hpp"


namespace std{
template<typename T,size_t L> class array;
}

namespace pulsar {

///Given a universe of atoms, \p DaU, in fractional coordinates and their space,
///\p DaSpace, converts the fractional coordinates to Cartesian coordinates
AtomSetUniverse Frac2Cart(const AtomSetUniverse& DaU, const Space& DaSpace);

///Given a universe of atoms, \p DaU, a set of SuperCell dimensions \p Dims,
///and the unit cell side lengths, \p Sides, creates a SuperCell
AtomSetUniverse MakeSuperCell(const AtomSetUniverse& DaU,
                              const std::array<size_t,3>& Dims,
                              const std::array<double,3>& Sides);

/** \brief Carves from a super cell a unit cell of the given dimension
 * 
 *  Given a super cell, \p SC, and the dimensions of the resulting unit cell,
 * \p Dims, our task is to carve out of the super cell a unit cell that does
 * not sever bonds.  To do this we first define three ranges, one for each
 * spatial dimension, such that for dimension i we have the range:
 * (MinScale*Dims[i],MaxScale*Dims[i]).  Each atom is tested to see if all of
 * their coordinates fall within that range.  If they do, that atom and all 
 * atoms bonded to it are added to the UC.  By default MaxScale and MinScale
 * are set such that we are plucking out the central cell of a 3 x 3 x 3
 * supercell.
 * 
 * \return An AtomSetUniverse of all atoms in the UC or bonded to an atom in
 *         the UC.
 */ 
AtomSetUniverse CarveUC(const AtomSetUniverse& SC,
                        const std::array<double,3>& Sides,
                        double MaxScale=1.0,
                        double MinScale=2.0);

/** \brief Cleans a unit cell such that when used to create a supercell
 *         that supercell will not contain atoms that sit on top of one
 *         another.
 * 
 *  In general when we apply CarveUC we will grab the periodic replica of some
 *  of the molecules in our UC.  If we use this UC to make a supercell we will
 *  end up with molecules on top of one another.  This function throws away
 *  the periodic replicas (which molecule is real and which is the replica
 *  is not defined for this function).  At the moment we do this via an
 *  iteratrive method, find two molecules sitting on top of one another,
 *  throw one away, see if our newly created supercell is ok.  If not find
 *  another replica and throw it away, etc.
 * 
 *  In theory I think we could do this non iteratively by identifying a replica
 *  and then translating it to determine which molecules it is symmetry
 *  equivalent to and then deleting all but one of them.  At the moment this
 *  function is fine speed wise so I'm not worrying about it, but this is one
 *  avenue to pursue if this becomes a bottlenecek.
 */
AtomSetUniverse CleanUC(const AtomSetUniverse& UC,
        const std::array<double,3>& Sides
      );

}//End namespace pulsar
#endif /* PULSAR_GHUARD_CRYSTALFUNCTIONS_HPP */

