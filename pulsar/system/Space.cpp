/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <limits>
#include <cmath>
#include "pulsar/system/Space.hpp"

namespace pulsar{
namespace system{

const double Inf=std::numeric_limits<double>::infinity();

bool Space::is_periodic()const{
    for(double s : lattice_sides)
        if(std::isfinite(s))return true;
    return false;
}

//double Space::Volume()const{
//    return !is_periodic()?Inf:sqrt(1-cos(Lattic))
//}

Space::Space():Space({90.0,90.0,90.0},{Inf,Inf,Inf}){}

}}//End namespaces

