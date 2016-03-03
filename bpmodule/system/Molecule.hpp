#ifndef BPMODULE_GUARD_SYSTEM__MOLECULE_HPP_
#define BPMODULE_GUARD_SYSTEM__MOLECULE_HPP_

#include <map>
#include "bpmodule/system/Atom.hpp"
#include "bpmodule/math/MathSet.hpp"
#include "bpmodule/math/PointManipulation.hpp"

// Instantiated in the cpp file
extern template class bpmodule::math::Universe<bpmodule::system::Atom>;
extern template class bpmodule::math::MathSet<bpmodule::system::Atom>;


// forward declare
namespace bpmodule {
namespace basisset {
class BasisSet;
}}


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

        double charge_;
        double multiplicity_;
        double nelectrons_;

        // Sets charge, multiplicity, and nelectrons to ones
        // determined from the Atoms in this set
        void SetDefaults_(void);

    public:
        typedef Atom::CoordType CoordType;
        typedef std::map<std::string, Molecule> FragMapType;
        typedef AtomSet::value_type value_type;
        typedef AtomSet::const_iterator const_iterator;
        typedef AtomSet::SelectorFunc SelectorFunc;
        typedef AtomSet::TransformerFunc TransformerFunc;

        /*! \brief Construct a molecule given a universe
         * 
         * \param [in] fill Make this molecule contain all the elements of the universe
         */
        Molecule(std::shared_ptr<const AtomSetUniverse> universe, bool fill);
        Molecule(const AtomSetUniverse& universe,bool fill);

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
        double GetSumCharge(void) const;
        void SetCharge(double charge);

        double GetNElectrons(void) const;
        double GetSumNElectrons(void) const;
        void SetNElectrons(double nelectrons);

        double GetMultiplicity(void) const;
        void SetMultiplicity(double m);

        // Get an atom
        bool HasAtom(size_t atomidx) const;
        Atom GetAtom(size_t atomidx) const;

        // Printing
        std::string ToString(void) const;


        // Iteration over all atoms in this Molecule/Set
        const_iterator begin(void) const { return atoms_.begin(); }
        const_iterator end(void) const { return atoms_.end(); }


        // Centers
        math::Point CenterOfMass(void) const;
        math::Point CenterOfNuclearCharge(void) const;

        // Set operations
        void Insert(const Atom & atom);
        Molecule Partition(SelectorFunc selector) const;
        Molecule Transform(TransformerFunc transformer) const;
        Molecule Complement(void) const;
        Molecule Intersection(const Molecule & rhs) const;
        Molecule Union(const Molecule & rhs) const;
        Molecule Difference(const Molecule & rhs) const;
        

        // Basis set stuff
        basisset::BasisSet GetBasisSet(const std::string & basislabel) const;


        // Manipulations
        template<typename VectorType>
        Molecule Translate(const VectorType & vec) const
        {
            return Transform(std::bind(math::TranslatePoint_Copy<Atom, VectorType>, std::placeholders::_1, vec));
        }

        template<typename MatrixType>
        Molecule Rotate(const MatrixType & mat) const
        {
            return Transform(std::bind(math::RotatePoint_Copy<Atom, MatrixType>, std::placeholders::_1, mat));
        }



};

inline std::ostream& operator<<(std::ostream& os,const Molecule& Mol){
    return os<<Mol.ToString();
}


} // close namespace system
} // close namespace bpmodule


#endif
