#ifndef BPMODULE_GUARD_MOLECULE__ATOM_HPP_
#define BPMODULE_GUARD_MOLECULE__ATOM_HPP_

//#include <array>
//typedef std::array<double, 3> CoordType;
#include <iostream>
#include <string>
#include "bpmodule/math/Point.hpp"
#include "bpmodule/util/Enumeration.hpp"

namespace bpmodule {
namespace system {


/*struct Atom
{
    unsigned long id;   //!< Unique id of this center
    int z;              //!< Atomic Z number
    int isonum;         //!< Isotope number
    CoordType xyz;      //!< XYZ coordinates

    //! Atomic mass for this atom/isotope
    double Mass(void) const;

    //! Name of the element
    std::string Name(void) const;

    //! Symbol of the element;
    std::string Symbol(void) const;
};*/
///@todo int to double and double to int conversions through math casts

class AtomProperty: public util::Enumeration<AtomProperty>{
private:
    AtomProperty(const std::string& Name):
        util::Enumeration<AtomProperty>(Name){}
public:
    static const AtomProperty Mass;
    static const AtomProperty Z;
    static const AtomProperty Charge;
    static const AtomProperty Mult;
    static const AtomProperty NElec;
};

    
///Wrapper class around math::Point for an atom
class Atom: public math::Point<AtomProperty,double>{
    private:
        ///Typedef of the base type
        typedef math::Point<AtomProperty,double> Base_t;
    public:
        ///Makes an atom with atomic number Z, at {x,y,z}, default ghost at
        ///origin
        Atom(size_t ZIn=0,double x=0.0,double y=0.0,double z=0.0);

        ///@{ Setters
        ///Sets the mass to m
        void SetMass(double m){AddWeight(AtomProperty::Mass,m);}
        ///Sets the charge to q and multiplicity to m
        void SetChargeAndMult(double q,size_t m){
            AddWeight(AtomProperty::Charge,q);
            AddWeight(AtomProperty::Mult,(double)m);
        }
        ///Sets the number of electrons
        void SetNElec(double N){AddWeight(AtomProperty::NElec,N);}
        ///@}
        
        
        ///@{ Getters
        ///Returns the atomic number
        size_t Z()const{return (size_t)Weight(AtomProperty::Z);}
        ///Returns the atomic symbol
        std::string Symbol()const;
        ///Returns the mass
        double Mass()const{return Weight(AtomProperty::Mass);}
        ///Returns the charge
        double Charge()const{return Weight(AtomProperty::Charge);}
        ///Returns the multiplicity of the atom
        size_t Mult()const{return (size_t)Weight(AtomProperty::Mult);}
        ///Returns the number of electrons
        double NElec()const{return Weight(AtomProperty::NElec);}
        ///@}
        
        ///Useful for printing the Atom out
        std::string ToString()const;
    };
    
    inline std::ostream& operator<<(std::ostream& os,const Atom& A){
        return os<<A.ToString();
    }


} // close namespace system
} // close namespace bpmodule

#endif

