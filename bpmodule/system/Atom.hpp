#ifndef BPMODULE_GUARD_MOLECULE__ATOM_HPP_
#define BPMODULE_GUARD_MOLECULE__ATOM_HPP_

//#include <array>
//typedef std::array<double, 3> CoordType;
#include <iostream>
#include <string>
#include "bpmodule/math/Point.hpp"

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
    
///Wrapper class around math::Point for an atom
class Atom: public math::Point<std::string,double>{
    private:
        ///Typedef of the base type
        typedef math::Point<std::string,double> Base_t;
    public:
        ///Makes an atom with atomic number Z, at {x,y,z}, default ghost at
        ///origin
        Atom(size_t Z=0,double x=0.0,double y=0.0,double z=0.0);

        ///@{ Setters
        ///Sets the mass to m
        void SetMass(double m){AddWeight("MASS",m);}
        ///Sets the charge to q and multiplicity to m
        void SetChargeAndMult(double q,size_t m){
            AddWeight("CHARGE",q);
            AddWeight("MULT",(double)m);
        }
        ///Sets the number of electrons
        void SetNElec(double N){AddWeight("NELEC",N);}
        ///@}
        
        
        ///@{ Getters
        ///Returns the atomic number
        size_t Z()const{return (size_t)Weight("Z");}
        ///Returns the atomic symbol
        std::string Symbol()const;
        ///Returns the mass
        double Mass()const{return Weight("MASS");}
        ///Returns the charge
        double Charge()const{return Weight("CHARGE");}
        ///Returns the multiplicity of the atom
        size_t Mult()const{return (size_t)Weight("MULT");}
        ///Returns the number of electrons
        double NElec()const{return Weight("NELEC");}
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

