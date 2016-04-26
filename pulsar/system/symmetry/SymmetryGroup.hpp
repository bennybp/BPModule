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
#include "pulsar/system/symmetry/SymmetryElements.hpp"

namespace pulsar {
namespace system{

///Base class for a group of symmetry elements
struct SymmetryGroup{
    const std::unordered_set<SymmetryElement> Elems;
    const std::string SSymbol;
    const std::string HMSymbol;
    SymmetryGroup(const std::unordered_set<SymmetryElement>& Es,
                  const std::string SSym,const std::string HMSym):
                  Elems(Es),SSymbol(SSym),HMSymbol(HMSym){}
    void Print(std::ostream& os)const{os<<SSymbol;}
};

inline std::ostream& operator<<(std::ostream& os, const SymmetryGroup& G){
    G.Print(os);
    return os;
}

///Follows flow-chart logic to assign point group
SymmetryGroup AssignGroup(const std::unordered_set<SymmetryElement>& Elems);

}//End namespace system
}//End namespace pulsar
#endif /* PULSAR_GHUARD_SYMMETRYGROUP_HPP */

