#ifndef BPMODULE_GUARD_MOLECULE__ATOM_HPP_
#define BPMODULE_GUARD_MOLECULE__ATOM_HPP_

#include <iostream>
#include <set>
#include <string>
#include "bpmodule/math/Point.hpp"
#include "bpmodule/math/Cast.hpp"
#include "bpmodule/util/Enumeration.hpp"

namespace bpmodule {
namespace system {


/*! \brief A center in a molecule
 * 
 * Follows PointConcept via derivation from math::Point
 */
class Atom : public math::Point
{
    private:
        int Z_;        //!< Atomic Z number (as integer. Also stored as a (double) weight)
        int isonum_;   //!< Isotope number

        double charge_;
        double multiplicity_;
        double nelectrons_;

        std::set<std::string> tags_;

    public:
        typedef std::set<std::string> TagsType;
        typedef math::Point::CoordType CoordType;

        // constructor
        Atom(CoordType xyz, int Z, int isonum,
             double charge, double multiplicity, double nelectrons, const TagsType & tags)
        {
            // we do it this way in case we change where the info is stored
            SetCoords(xyz);
            SetZ(Z);
            SetIsonum(isonum);
            SetCharge(charge);
            SetMultiplicity(multiplicity);
            SetNElectrons(nelectrons);
            SetTags(tags);
        }



        /*
         * I'm aware that a class with lots of getters/setters is bad
         * form. However, we are leaving it open to future optimizations,
         * such as storing the data elsewhere
         */
        int GetZ(void) const noexcept { return Z_; }
        void SetZ(int Z) noexcept { Z_ = Z; }

        int GetIsonum(void) const noexcept { return isonum_; }
        void SetIsonum(int isonum) noexcept { isonum_ = isonum; }

        double GetCharge(void) const noexcept { return charge_; }
        void SetCharge(double charge) noexcept { charge_ = charge; }

        double GetMultiplicity(void) const noexcept { return charge_; }
        void SetMultiplicity(double m) noexcept { multiplicity_ = m; }

        double GetNElectrons(void) const noexcept { return nelectrons_; }
        void SetNElectrons(double n) noexcept { nelectrons_ = n; }

        TagsType GetTags(void) const { return tags_; }
        void SetTags(const TagsType & tags) { tags_ = tags; }
        bool HasTag(const std::string & tag) const { return tags_.count(tag); }

        bool operator==(const Atom & rhs) const;
        bool operator!=(const Atom & rhs) const;

        //! Lookups in Atomic Info
        double GetMass(void) const;

        double GetIsotopeMass(void) const;

        //! Name of the element
        std::string GetName(void) const;

        //! Symbol of the element;
        std::string GetSymbol(void) const;
};

std::ostream& operator<<(std::ostream& os,const Atom& A);


/* \brief Create an atom given an ID, coordinates, and atomic number
 *
 * The rest of the data is filled in automatically
 */
Atom CreateAtom(Atom::CoordType xyz, int Z, const Atom::TagsType & tags);


/*! \copydocs CreateAtom(size_t, Atom::CoordType, int) */
Atom CreateAtom(double x, double y, double z, int Z, const Atom::TagsType & tags);



/* \brief Create an atom given an ID, coordinates, atomic number, and isotope number
 *
 * The rest of the data is filled in automatically
 */
Atom CreateAtom(Atom::CoordType xyz, int Z, int isonum, const Atom::TagsType & tags);


/*! \copydocs CreateAtom(size_t, Atom::CoordType, int, int) */
Atom CreateAtom(double x, double y, double z, int Z, int isonum, const Atom::TagsType & tags);



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

