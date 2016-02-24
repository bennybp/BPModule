#ifndef BPMODULE_GUARD_MOLECULE__MOLECULE_HPP_
#define BPMODULE_GUARD_MOLECULE__MOLECULE_HPP_

#include <vector>

#include "bpmodule/system/Atom.hpp"
#include "bpmodule/math/MathSet.hpp"
#include "bpmodule/math/PointManipulation.hpp"

namespace bpmodule {
namespace system {


class Molecule
{
    private:
        unsigned long curid_;
        std::vector<Atom> atoms_;

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


        Atom GetAtom(size_t i) const;
        void SetAtom(size_t i, const Atom & a);
        void AddAtom(const Atom & a);

        int NAtoms(void) const noexcept;


        // Some molecule properties
        double GetCharge(void) const;
        double GetNElectrons(void) const;


        // Iteration over all atoms
        typedef Atom value_type;
        iterator begin(void);
        iterator end(void);
        const_iterator begin(void) const;
        const_iterator end(void) const;


        // Manipulations
        template<typename VectorType>
        Molecule Translate(const VectorType & vec) const
        {
            return math::TranslatePointContainer_Copy(*this, vec);
        }

        template<typename MatrixType>
        Molecule Rotate(const MatrixType & mat) const
        {
            return math::RotatePointContainer_Copy(*this, mat);
        }

        // Centers
        math::Point CenterOfMass(void) const;
        math::Point CenterOfNuclearCharge(void) const;

};



} // close namespace system
} // close namespace bpmodule


#endif
