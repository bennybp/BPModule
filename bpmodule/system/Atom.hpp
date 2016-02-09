#ifndef BPMODULE_GUARD_MOLECULE__ATOM_HPP_
#define BPMODULE_GUARD_MOLECULE__ATOM_HPP_

#include <iostream>
#include <string>
#include "bpmodule/math/Point.hpp"
#include "bpmodule/math/Cast.hpp"
#include "bpmodule/system/AtomicInfo.hpp"
#include "bpmodule/util/Enumeration.hpp"

namespace bpmodule {
namespace system {


enum class AtomWeights
{
    Mass,
    Z,
    Charge,
    Multiplicity,
    NElectrons,
    NWEIGHTS
};



class Atom : public math::WeightedPoint<AtomWeights>
{
    private:
        typedef math::WeightedPoint<AtomWeights> Point_t;

        size_t id_;    //!< Unique id of this center
        int Z_;        //!< Atomic Z number (as integer. Also stored as a (double) weight)
        int isonum_;   //!< Isotope number

    public:
        //! Constructor
        Atom(size_t id, int Z, CoordType xyz)
            : Point_t(xyz), id_(id)
        {
            SetZ(Z);
            SetIsonum(MostCommonIsotopeFromZ(Z));
            SetMass(AtomicMassFromZ(Z));
            SetMultiplicity(AtomicMultiplicityFromZ(Z));
            SetNElectrons(Z);
            SetCharge(0);
        }


        //! Constructor with coordinates separated
        Atom(size_t id, int Z, double x, double y, double z)
            : Atom(id, Z, {x,y,z})
        { }


        //! Default constructor
        Atom() : Atom(0, 0, {0, 0, 0})  { }



        //! ID of this atom
        size_t GetID(void) const noexcept  { return id_; }

        //! Set the ID
        void SetID(size_t id) noexcept  { id_ = id; }


        //! Atomic Z number
        int GetZ(void) const noexcept { return Z_; }
        void SetZ(int Z) noexcept
        {
            // set the integer and the weight
            Z_ = Z;

            //! \todo Replace with other exact cast?
            Point_t::Weight(AtomWeights::Z) = math::numeric_cast<double>(Z);
        }


        //! Isotope number
        int GetIsonum(void) const noexcept { return isonum_; }
        void SetIsonum(int isonum) noexcept { isonum_ = isonum; }


        //! Atomic mass for this atom
        double GetMass(void) const      { return Point_t::Weight(AtomWeights::Mass); }
        void SetMass(double mass)       { Point_t::Weight(AtomWeights::Mass) = mass; }

        //! Charge on this atom
        double GetCharge(void) const      { return Point_t::Weight(AtomWeights::Charge); }
        void SetCharge(double mass)       { Point_t::Weight(AtomWeights::Charge) = mass; }

        //! Multiplicity on this atom
        double GetMultiplicity(void) const      { return Point_t::Weight(AtomWeights::Multiplicity); }
        void SetMultiplicity(double mult)       { Point_t::Weight(AtomWeights::Multiplicity) = mult; }

        //! NElectrons on this atom
        double GetNElectrons(void) const      { return Point_t::Weight(AtomWeights::NElectrons); }
        void SetNElectrons(double nelectrons) { Point_t::Weight(AtomWeights::NElectrons) = nelectrons; }

        //! Name of the element
        std::string Name(void) const;

        //! Symbol of the element;
        std::string Symbol(void) const;
};



/*
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
        
        ///Returns true if two Atoms are equal
        bool operator==(const Atom& RHS)const;
        ///Returns true if two Atoms are not equal
        bool operator!=(const Atom& RHS)const{
            return !((*this)==RHS);
        }
        
        
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
*/

} // close namespace system
} // close namespace bpmodule

#endif

