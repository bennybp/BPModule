/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/** \file CrystalSystem.hpp
 *  \brief Write Me!!!!!!
 *  \author Ryan M. Richard
 *  \version 1.0
 *  \date April 28, 2016
 */

#ifndef PULSAR_GUARD_CRYSTALSYSTEM_HPP
#define PULSAR_GUARD_CRYSTALSYSTEM_HPP

#include<array>
#include<string>

namespace pulsar {
namespace system {

struct CrystalSystem{
    const std::array<double,3> Angles_;
    const std::array<double,3> Sides_;
    const std::string Name_;
    CrystalSystem(const std::array<double,3>& Angles,
                  const std::array<double,3>& Sides,const std::string Name):
                  Angles_(Angles),Sides_(Sides),Name_(Name){}
};

///Helper class for triclinic lattice (no restrictions)
struct Triclinic: public CrystalSystem{
    Triclinic(const std::array<double,3>& As,const std::array<double,3>& Ss):
         CrystalSystem(As,Ss,"Triclinic"){}
};

///Helper for monoclinic lattice (alpha=gamma=90.0)
struct Monoclinic: public CrystalSystem{
    Monoclinic(const std::array<double,3>& As,const std::array<double,3>& Ss):
         CrystalSystem(As,Ss,"Monoclinic"){}
    Monoclinic(double beta,const std::array<double,3>& Ss):
         Monoclinic({90.0,beta,90.0},Ss){}
};

///Helper for orthorhombic lattice (alpha=beta=gamma=90.0)
struct Orthorhombic: public CrystalSystem{
    Orthorhombic(const std::array<double,3>& As,const std::array<double,3>& Ss):
         CrystalSystem(As,Ss,"Orthorhombic"){}
    Orthorhombic(const std::array<double,3>& Ss):
         Orthorhombic({90.0,90.0,90.0},Ss){}
};

///Helper for Tetragonal lattice (Orthorhombic plus a=b)
struct Tetragonal: public CrystalSystem{
    Tetragonal(const std::array<double,3>& As,const std::array<double,3>& Ss):
         CrystalSystem(As,Ss,"Tetragonal"){}
    Tetragonal(double AorB,double c):
         Tetragonal({90.0,90.0,90.0},{AorB,AorB,c}){}
};

///Helper for rhombohedral lattice (a=b=c and alpha=beta=gamma)
struct Rhombohedral: public CrystalSystem{
    Rhombohedral(const std::array<double,3>& As,const std::array<double,3>& Ss):
         CrystalSystem(As,Ss,"Rhombohedral"){}
    Rhombohedral(double Angle,double Side):
         Rhombohedral({Angle,Angle,Angle},{Side,Side,Side}){}
};

///Helper for hexagonal lattice (alpha=beta=90 gamma=120, a=b)
struct Hexagonal: public CrystalSystem{
       Hexagonal(const std::array<double,3>& As,const std::array<double,3>& Ss):
         CrystalSystem(As,Ss,"Hexagonal"){}
       Hexagonal(double AorB,double C):
         Hexagonal({90.0,90.0,120.0},{AorB,AorB,C}){} 
};

///Helper for cubic lattice (alpha=beta=gamma=90 a=b=c)
struct Cubic: public CrystalSystem{
       Cubic(const std::array<double,3>& As,const std::array<double,3>& Ss):
        CrystalSystem(As,Ss,"Cubic"){}
       Cubic(double Side):
         Cubic({90.0,90.0,90.0},{Side,Side,Side}){} 
};

///Assigns your crystal system allowing for a percent tolerance of /p Tol
CrystalSystem AssignCrystalSystem(const std::array<double,3>& Angles,
                                  const std::array<double,3>& Sides,
                                  double Tol=0.05);

}}//End namespace pulsar
#endif /* PULSAR_GHUARD_CRYSTALSYSTEM_HPP */

