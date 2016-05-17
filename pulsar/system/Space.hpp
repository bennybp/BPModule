/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/** \file Space.hpp
 *  \brief Write Me!!!!!!
 *  \author Ryan M. Richard
 *  \version 1.0
 *  \date April 29, 2016
 */

#ifndef PULSAR_GUARD_SPACE_HPP
#define PULSAR_GUARD_SPACE_HPP

#include <array>

namespace pulsar {
namespace system {

///Class describing the space around a molecule
struct Space{
    ///Sides (a.u.) of our lattice
    std::array<double,3> LatticeSides;
    ///Angles (degrees) of our lattice
    std::array<double,3> LatticeAngles;
    
    ///True if sides are not infinite
    bool IsPeriodic()const;
    
    ///Makes a space with custom lattice
    Space(const std::array<double,3>& A,const std::array<double,3>& S):
        LatticeSides(S),LatticeAngles(A){}
    
    ///Makes default vacuous space
    Space();
};

}}//End namespace pulsar
#endif /* PULSAR_GHUARD_SPACE_HPP */

