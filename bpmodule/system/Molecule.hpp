#ifndef BPMODULE_GUARD_MOLECULE__MOLECULE_HPP_
#define BPMODULE_GUARD_MOLECULE__MOLECULE_HPP_

#include <map>
#include "bpmodule/system/Atom.hpp"
#include "bpmodule/math/MathSet.hpp"
#include "bpmodule/math/PointManipulation.hpp"

namespace bpmodule {
namespace system {


typedef math::Universe<Atom> AtomSetUniverse;
typedef math::MathSet<Atom> AtomSet;


class Molecule : public math::MathSet<Atom>
{
    private:
        typedef math::MathSet<Atom> Base_t;

    public:
        typedef std::vector<Atom>::iterator       iterator;
        typedef std::vector<Atom>::const_iterator const_iterator;
        typedef Atom::CoordType CoordType;

        Molecule(std::shared_ptr<const AtomSetUniverse> universe, bool fill);

        // compiler generated ok
        Molecule(const Molecule & rhs)             = default;
        Molecule(Molecule && rhs)                  = default;
        Molecule & operator=(const Molecule & rhs) = default;
        Molecule & operator=(Molecule && rhs)      = default;

        int NAtoms(void) const noexcept;


        // Some molecule properties
        double GetCharge(void) const;
        double GetNElectrons(void) const;

        std::string ToString(void) const;

        // Iteration over all atoms in this Molecule/Set
        typedef Atom value_type;
        using Base_t::begin;
        using Base_t::end;


        // General info
        std::set<std::string> AllTags(void) const;
        std::map<std::string, Molecule> Fragments(void) const;


        // Manipulations
        template<typename VectorType>
        Molecule Translate(const VectorType & vec) const
        {
            return TransformMathSet(*this,
                                    std::bind(math::TranslatePoint_Copy<Atom, VectorType>, std::placeholders::_1, vec));
        }

        template<typename MatrixType>
        Molecule Rotate(const MatrixType & mat) const
        {
            return TransformMathSet(*this,
                                    std::bind(math::RotatePoint_Copy<Atom, MatrixType>, std::placeholders::_1, mat));
        }

        // Centers
        math::Point CenterOfMass(void) const;
        math::Point CenterOfNuclearCharge(void) const;

};



} // close namespace system
} // close namespace bpmodule


#endif
