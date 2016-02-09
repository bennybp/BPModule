#ifndef BPMODULE_GUARD_MOLECULE__MOLECULE_HPP_
#define BPMODULE_GUARD_MOLECULE__MOLECULE_HPP_

#include <vector>

#include "bpmodule/system/Atom.hpp"
#include "bpmodule/math/MathSet.hpp"
#include "bpmodule/math/Point.hpp"

namespace bpmodule {
namespace system {
/*
class Molecule: 
public math::MathSet<Atom,math::PointStorage<Atom>>{
private:
    ///The charge of the molecule
    double Charge_;
public:
    Molecule():Charge_(0.0){}
    void AddAtom(const Atom& A);
    std::string ToString()const;
    
    double Charge()const noexcept{return Charge_;}
    
    void SetCharge(double Q){Charge_=Q;}
};

inline std::ostream& operator<<(std::ostream& os,const Molecule& Mol){
    return os<<Mol.ToString();
}
*/

class Molecule
{
    private:
        unsigned long curid_;
        std::vector<Atom> atoms_;

        double charge_;

    public:
        typedef std::vector<Atom>::iterator       iterator;
        typedef std::vector<Atom>::const_iterator const_iterator;
        typedef Atom::CoordType CoordType;

        Molecule(void);

        // compiler generated ok
        Molecule(const Molecule & rhs)             = default;
        Molecule(Molecule && rhs)                  = default;
        Molecule & operator=(const Molecule & rhs) = default;
        Molecule & operator=(Molecule && rhs)      = default;


        void AddAtom(int Z, CoordType xyz);

        Atom & GetAtom(int i);
        const Atom & GetAtom(int i) const;

        int NAtoms(void) const noexcept;


        // Iteration over all atoms
        const_iterator begin(void) const;
        const_iterator end(void) const;

        // Some molecule properties
        double Charge(void) const noexcept { return charge_; } 
        void SetCharge(double charge) noexcept { charge_ = charge; }


};



} // close namespace system
} // close namespace bpmodule


#endif
