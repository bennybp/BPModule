/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Geometry.hpp
 * Author: richard
 *
 * Created on April 15, 2016, 3:43 PM
 */

#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include<array>
#include<cmath>
#include"bpmodule/util/Constants.hpp"//For Pi
namespace bpmodule{
namespace math{


//Returns rotation matrix about the unit vector by \p n degrees
template<typename T>
std::array<double,9> Rotation(const T& Axis, double ndegrees){
    const double angle=ndegrees*PI_D/180.0,x=Axis[0],y=Axis[1],z=Axis[2];
    const double c=cos(angle),s=sin(angle);
    double c1=1-c;
    return {c+x*x*c1, x*y*c1-z*s,x*z*c1+y*s,
                y*x*c1+z*s,c+y*y*c1,y*z*c1-x*s,
                z*x*c1-y*s,z*y*c1+x*s,c+z*z*c1};
}

///Reflection through plane normal to unit vector \p Norm
template<typename T>
std::array<double,9> Reflection(const T& Norm){
        const double x=Norm[0],y=Norm[1],z=Norm[2];
        return {1-2*x*x, -2*x*y, -2*x*z,
                 -2*x*y,1-2*y*y, -2*y*z,
                 -2*x*z, -2*y*z,1-2*z*z};
}

///Rotation about \p Axis by \p n degrees followed by reflection
///perpendicular to it
template<typename T>
std::array<double,9> RotoReflection(const T& P,double n){
        std::array<double,9> Rot=Rotation(P,n),Ref=Reflection(P),Result={};
        for(size_t i=0;i<3;++i)
            for(size_t j=0;j<3;++j)
                for(size_t k=0;k<3;++k)
                    Result[i*3+j]+=Ref[i*3+k]*Rot[k*3+j];
        return Result;
}



}}//End namespaces


#endif /* GEOMETRY_HPP */

