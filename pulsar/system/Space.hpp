/** \file Space.hpp
 *  \brief Write Me!!!!!!
 *  \author Ryan M. Richard
 *  \version 1.0
 *  \date April 29, 2016
 */

#ifndef PULSAR_GUARD_SPACE_HPP
#define PULSAR_GUARD_SPACE_HPP

#include <array>
#include <limits>
namespace pulsar {

///Class describing the space around a molecule
struct Space{
    ///Sides (a.u.) of our lattice
    std::array<double,3> lattice_sides;
    ///Angles (radians) of our lattice
    std::array<double,3> lattice_angles;
    ///For your convenience this is the value of infinity
    constexpr static const double infinity=std::numeric_limits<double>::infinity();
    
    ///True if sides are not infinite
    bool is_periodic()const;
    
    ///Returns the volume of the cell
    //double Volume()const;
    
    
    ///Makes a periodic space
    Space(const std::array<double,3>& A,const std::array<double,3>& S):
        lattice_sides(S),lattice_angles(A){}
    
    ///Makes default vacuous space
    Space();
    
    ///Defaults are fine, all deep copies
    Space(const Space&)=default;
    Space(Space&&)=default;
    Space& operator=(const Space&)=default;
    Space& operator=(Space&&)=default;
    
    bool operator==(const Space& other)const;
    bool operator!=(const Space& other)const{
        return !((*this)==other);
    }
};

}//End namespace pulsar
#endif /* PULSAR_GHUARD_SPACE_HPP */

