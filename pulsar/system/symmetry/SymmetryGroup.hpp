/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/** \file SymmetryGroup.hpp
 *  \brief Write Me!!!!!!
 *  \author Ryan M. Richard
 *  \version 1.0
 *  \date April 20, 2016
 */

#ifndef PULSAR_GUARD_SYMMETRYGROUP_HPP
#define PULSAR_GUARD_SYMMETRYGROUP_HPP

#include <unordered_set>
#include <string>
#include <iostream>
#include "pulsar/system/symmetry/SymmetryElement.hpp"

namespace pulsar {
namespace system{

///Base class for a group of symmetry elements
struct SymmetryGroup{
    ///The set of elements in this group
    const std::unordered_set<SymmetryElement> Elems;
    ///Schoenflies symbol
    const std::string SSymbol;
    ///Hermann-Mauguin symbol
    const std::string HMSymbol;
    ///Returns the order of the group, throws if group is infinite order
    size_t Order()const;
    ///True if this is a finite group
    bool Finite()const;
    ///Builds a new SymmetryGroup, auto adds identity
    SymmetryGroup(const std::unordered_set<SymmetryElement>& Es,
                  const std::string SSym,const std::string HMSym);
    ///Prints the group the given buffer
    void Print(std::ostream& os)const{os<<SSymbol;}
};

inline std::ostream& operator<<(std::ostream& os, const SymmetryGroup& G){
    G.Print(os);
    return os;
}

///Follows flow-chart logic to assign point group
SymmetryGroup AssignGroup(const std::unordered_set<SymmetryElement>& Elems);

///This namespace contains interfaces whose entire purpose is to set up the
///SymmetryGroup correctly and then be sliced off
namespace PointGroup{//These names are short so namespace protect them

#define PGMacro(name)\
    struct name: public SymmetryGroup{\
    name(const std::unordered_set<SymmetryElement>&,size_t);\
    name(size_t);};


///Helper for cyclic groups
PGMacro(Cn)
///Helper for pyramidal groups
PGMacro(Cnv)
///Helper for cyclic reflection groups
PGMacro(Cnh)
///Helper for improper rotation group (n must be even or you have a Cnh group)
PGMacro(Sn)
///Helper for dihedral groups
PGMacro(Dn)
///Helper for prismatic groups
PGMacro(Dnh)
///Helper for antiprismatic groups
PGMacro(Dnd)

#undef PGMacro
struct Cs:public SymmetryGroup{
        Cs(const std::unordered_set<SymmetryElement>&Es):
    SymmetryGroup(Es,"Cs","m"){}
        Cs();
        
};
struct Ci:public SymmetryGroup{
        Ci(const std::unordered_set<SymmetryElement>&Es):
    SymmetryGroup(Es,"Ci","[1]"){}
        Ci();
};

///Helper for linear w/ no inversion
struct Coov:public SymmetryGroup{
    Coov(const std::unordered_set<SymmetryElement>&Es):
    SymmetryGroup(Es,"Coov","oom"){}
    Coov();
};

///Helper for linear w/ inversion
struct Dooh:public SymmetryGroup{
    Dooh(const std::unordered_set<SymmetryElement>& Es):
    SymmetryGroup(Es,"Dooh","oo/mm"){}
    Dooh();
};

struct T:public SymmetryGroup{
    T(const std::unordered_set<SymmetryElement>& Es):
    SymmetryGroup(Es,"T","23"){}
};

struct Th:public SymmetryGroup{
    Th(const std::unordered_set<SymmetryElement>& Es):
    SymmetryGroup(Es,"Th","m[3]"){}
};

struct Td:public SymmetryGroup{
    Td(const std::unordered_set<SymmetryElement>& Es):
    SymmetryGroup(Es,"Td","[4]3m"){}
};

struct O:public SymmetryGroup{
    O(const std::unordered_set<SymmetryElement>& Es):
    SymmetryGroup(Es,"O","432"){}
};

struct Oh:public SymmetryGroup{
    Oh(const std::unordered_set<SymmetryElement>& Es):
    SymmetryGroup(Es,"Oh","m[3]m"){}
};

struct I:public SymmetryGroup{
    I(const std::unordered_set<SymmetryElement>& Es):
    SymmetryGroup(Es,"I","532"){}
};

struct Ih:public SymmetryGroup{
    Ih(const std::unordered_set<SymmetryElement>& Es):
    SymmetryGroup(Es,"Ih","[53]m"){}
};

struct Kh:public SymmetryGroup{
    Kh(const std::unordered_set<SymmetryElement>& Es):
    SymmetryGroup(Es,"Kh","oo/moo"){}
    Kh():Kh(std::unordered_set<SymmetryElement>()){}
};

}//End namespace PointGroup

}//End namespace system
}//End namespace pulsar
#endif /* PULSAR_GHUARD_SYMMETRYGROUP_HPP */

