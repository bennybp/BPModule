/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <cmath>
#include "pulsar/system/Space.hpp"
#include "pulsar/constants.h"

namespace pulsar{
namespace system{
constexpr const double Space::infinity;
//::infinity=std::numeric_limits<double>::infinity();
bool Space::is_periodic()const{
    for(double s : lattice_sides)
        if(std::isfinite(s))return true;
    return false;
}

//double Space::Volume()const{
//    return !is_periodic()?Inf:sqrt(1-cos(Lattic))
//}

Space::Space():Space({PI/2,PI/2,PI/2},{infinity,infinity,infinity})
{
}

bool Space::operator==(const Space& RHS) const
{
   return ( lattice_sides == RHS.lattice_sides &&
           lattice_angles == RHS.lattice_angles
           );
}

}}//End namespaces

