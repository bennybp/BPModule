/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Symmetrizer.hpp
 * Author: richard
 *
 * Created on April 13, 2016, 3:51 PM
 */

#ifndef PULSAR_GUARD_SYSTEM__SYMMETRIZER_HPP_
#define PULSAR_GUARD_SYSTEM__SYMMETRIZER_HPP_

#include <map>
#include "pulsar/system/symmetry/SymmetryGroup.hpp"

namespace pulsar{
namespace system{
class System;

//typedef std::map<double,SymmetryGroup> RankedGroups;

class Symmetrizer{
    public:
        /** \brief Returns the SymmetryGroup the molecule most resembles
         *
         *  Internally we follow the algorithm of 
         *     Largent et al. JCC 33 (2012) 1637.
         * 
         *  This involves:
         *  1. Find principle moments of inertia and align molecule
         *  2. Determine degeneracy of moments
         *  3. Look for center of inversion at center of mass
         *  4. Find Cn axis along:
         *     a. Principal axis
         *     b. CoM and Atom
         *     c. CoM and "bond" midpoint between like atoms
         *  5. Any time a Cn is found we look for an Sn as they must be
         *     coincident with a Cn
         *  6. Reflections are looked for orthogonal to Cns.  In theory,
         *     we only have to look along C2s for O and I, but we don't know
         *     the group yet
         *  7. Reflections are looked for normal to CoM and an Atom
         *  8. Reflections are looked for normal to CoM and midpoint between
         *     two atoms.
         *  9. Additional rotations looked for through faces of cubic groups
         *  
         *  Notes:
         *  I don't know if it's my interpretation of this algorithm or if
         *  the algorithm really doesn't work, but as I understand it, it
         *  fails for Cs symmetry
         *  of CBrClH2.  There is a plane of reflection splitting the two
         *  hydrogens that is not orthogonal to a principal rotation axis
         *  (there aren't any), nor is it orthogonal to a CoM-atom axis,
         *  nor is it orthogonal to the vector bisecting any two like atoms
         *  (it's parallel to it).  I have revised step 8 to look for planes
         *  that split a bond and are orthogonal to the bond.
         * 
         * It also appears to fail for C2v H2O, which has two planes parallel to
         * the C2 axis and the H-H bisector.  So now I'm also checking
         * for planes parallel to and perpendicular to a principle axis.
         *  
         */
        SymmetryGroup GetSymmetry(const System& Mol)const;
};

}}//End namespaces
#endif /* SYMMETRIZER_HPP */

