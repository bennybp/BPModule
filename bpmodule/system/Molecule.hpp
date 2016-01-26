#ifndef BPMODULE_GUARD_MOLECULE__MOLECULE_HPP_
#define BPMODULE_GUARD_MOLECULE__MOLECULE_HPP_

#include <vector>

#include "bpmodule/system/Atom.hpp"
#include "bpmodule/math/MathSet.hpp"

namespace bpmodule {
namespace system {
    
class Molecule: 
    public math::MathSet<Atom,math::PointStorage<std::string,Atom>{
public:
    Molecule()=default;
    void AddAtom(const Atom& A);
    std::string ToString()const;
};

inline std::ostream& operator<<(std::ostream& os,const Molecule& Mol){
    return os<<Mol.ToString();
}


/*class Molecule
{
    public:
        typedef std::vector<Atom>::iterator       iterator;
        typedef std::vector<Atom>::const_iterator const_iterator;

        Molecule(void);

        // compiler generated ok
        Molecule(const Molecule & rhs)             = default;
        Molecule(Molecule && rhs)                  = default;
        Molecule & operator=(const Molecule & rhs) = default;
        Molecule & operator=(Molecule && rhs)      = default;


        void AddAtom(int Z, CoordType xyz);

        Atom GetAtom(int i) const;

        int NAtoms(void) const noexcept;


        // Iteration over all atoms
        const_iterator begin(void) const;
        const_iterator end(void) const;



    private:
        unsigned long curid_;
        std::vector<Atom> atoms_;
};*/



} // close namespace system
} // close namespace bpmodule


#endif
