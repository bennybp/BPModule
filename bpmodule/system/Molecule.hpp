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


class Molecule
{
    private:

        //! Actual storage of all the atoms
        AtomSet atoms_;

        // For use from transformations, etc
        Molecule(const AtomSet & atoms);


    public:
        typedef Atom::CoordType CoordType;
        typedef Atom::TagsType TagsType;
        typedef std::map<std::string, Molecule> FragMapType;
        typedef AtomSet::value_type value_type;
        typedef AtomSet::const_iterator const_iterator;

        /*! \brief Construct a molecule given a universe
         * 
         * \param [in] fill Make this molecule contain all the elements of the universe
         */
        Molecule(std::shared_ptr<const AtomSetUniverse> universe, bool fill);

        // compiler generated ok
        // Copies will share storage in the AtomSet, but have their
        // own sets of contained atoms
        Molecule(const Molecule & rhs)             = default;
        Molecule(Molecule && rhs)                  = default;
        Molecule & operator=(const Molecule & rhs) = default;
        Molecule & operator=(Molecule && rhs)      = default;


        // Some molecule properties
        int NAtoms(void) const;
        double GetCharge(void) const;
        double GetNElectrons(void) const;


        // Printing
        std::string ToString(void) const;


        // Iteration over all atoms in this Molecule/Set
        const_iterator begin(void) const { return atoms_.begin(); }
        const_iterator end(void) const { return atoms_.end(); }


        // Fragments
        TagsType GetAllTags(void) const;
        FragMapType GetAllFragments(void) const;
        Molecule GetFragment(const std::string & tag) const;

        // Centers
        math::Point CenterOfMass(void) const;
        math::Point CenterOfNuclearCharge(void) const;

        // Set operations
        Molecule Complement(void) const;

        // Manipulations
        template<typename VectorType>
        Molecule Translate(const VectorType & vec) const
        {
            AtomSet newatoms = atoms_.Transform(std::bind(math::TranslatePoint_Copy<Atom, VectorType>, std::placeholders::_1, vec));
            return Molecule(newatoms);
        }

        template<typename MatrixType>
        Molecule Rotate(const MatrixType & mat) const
        {
            AtomSet newatoms = atoms_.Transform(std::bind(math::RotatePoint_Copy<Atom, MatrixType>, std::placeholders::_1, mat));
            return Molecule(newatoms);
        }



};



} // close namespace system
} // close namespace bpmodule


#endif
