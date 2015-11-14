#ifndef _GUARD_MOLECULE_HPP_
#define _GUARD_MOLECULE_HPP_

#include <vector>

#include "bpmodule/molecule/Atom.hpp"

namespace bpmodule {
namespace molecule {


class Molecule
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
        const_iterator cbegin(void) const;
        const_iterator cend(void) const;



    private:
        unsigned long curid_;
        std::vector<Atom> atoms_;
};



} // close namespace molecule
} // close namespace bpmodule


#endif
