/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SymmetryElements.hpp
 * Author: richard
 *
 * Created on April 13, 2016, 3:38 PM
 */

#ifndef PULSAR_GUARD_SYSTEM__SYMMETRYELEMENTS_HPP_
#define PULSAR_GUARD_SYSTEM__SYMMETRYELEMENTS_HPP_

#include <array>
#include <string>
#include <sstream>

namespace pulsar{
namespace system{


///Base class for all symmetry elements
struct SymmetryElement{
    ///Matrix representation of this element
    const std::array<double,9> Elem;
        
    ///Schoenflies symbol
    const std::string SSymbol;
    
    ///Hermann-Mauguin symbol
    const std::string HMSymbol;
    
    ///Initializes all members of the group
    SymmetryElement(const std::array<double,9>& Op,
                    const std::string& SSym,const std::string& HMSym):
                    Elem(Op),SSymbol(SSym),HMSymbol(HMSym){}
    
    ///Performs a deep copy
    SymmetryElement(const SymmetryElement&)=default;
    
    /** \brief Returns true if this SymmetryElement's Elem is the same as 
     *         other's
     *
     *  I originally had this as an exact equality; however numeric precision
     *  caused that to fail so I introduced a threshold of 1.0e-5.  That
     *  threshold is too tight and I had to drop it to 1.0e-2.  Ultimately,
     *  these matrices are numerically quite noisy, even for structures that
     *  are reasonably close to their symmetric geometries.  The large
     *  in magnitude elements of these matrices are on the order of 0.1 to 1.0
     *  so this still represents a threshold that is 10 to 100% smaller.
     */
    bool operator==(const SymmetryElement& Other)const;
    
    ///Opposite of operator==()
    bool operator!=(const SymmetryElement& Other)const{
        return !((*this)==Other);
    }
    
    ///Prints out the Schoenflies symbol for the element
    std::ostream& Print(std::ostream& os)const;
};

///Reflection through the plane normal to \p Norm
struct MirrorPlane: public SymmetryElement{
    MirrorPlane(const std::array<double,3>& Norm);
};

///Proper rotation about \p Axis by 360 / \p n degrees, m times
struct Rotation: public SymmetryElement{
    Rotation(const std::array<double,3>& Axis,size_t n, size_t m);
};

///Improper rotation about \p Axis by 360 / \p n degrees, m times
struct ImproperRotation: public SymmetryElement{
    ImproperRotation(const std::array<double,3>& Op,size_t n, size_t m);
};

///Instance of symmetry element that is the identity
extern const SymmetryElement Identity;

///Instance of symmetry element that is a center of inversion
extern const SymmetryElement CoI;

///Allows printing of element
inline std::ostream& operator<<(std::ostream& os, const SymmetryElement& E){
    return E.Print(os);
}

}}//End namespaces

///Allows Symmetry elements to be hashed, although not terribly uniquely
namespace std{
template<> struct hash<pulsar::system::SymmetryElement>{
    size_t operator()(const pulsar::system::SymmetryElement& E)const{
        std::hash<std::string> h;
        return h(E.SSymbol);
    }
};
}

#endif /* SYMMETRYELEMENTS_HPP */

