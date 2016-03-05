/*\file
 *
 * \brief The molecule class (header)
*/


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


/*! \brief All atoms that may be used in a group of Molecules */
typedef math::Universe<Atom> AtomSetUniverse;



/*! \brief A collection of Atoms
 *
 * Once atoms are placed in a Molecule (or AtomSetUniverse), they
 * cannot be changed in any way. This allows for consistency when
 * atoms are shared among may Molecule objects (via the universe).
 * Changing the Atoms themselves require making copies of the Atom.
 * This can be accomplished easily through the Molecule::Transform function,
 * which returns a new Molecule containing a new universe. This new molecule
 * is completely divorced from the original and does not share any resources
 * with it.
 *
 * The only thing modifiable about a non-const Molecule is which Atoms (from
 * of its universe) it contains.
 * Assignment and copy construction will result in Molecules sharing the
 * universe, but with separate lists of atoms they contain.
 *
 * This Molecule object will behave like a MathSet, allowing for
 * unions, intersections, complements, etc.
 */
class Molecule
{
    private:
        //! How atoms are actually stored in the Molecule
        typedef math::MathSet<Atom> AtomSet;

        //! Actual storage of all the atoms
        AtomSet atoms_;

        // For use from transformations, etc
        Molecule(const AtomSet & atoms);

        double charge_;       //!< Total charge on this molecule
        double multiplicity_; //!< Total multiplicity of the molecule
        double nelectrons_;   //!< Total number of electrons in the molecule 

        /* \brief Sets charge, multiplicity, and nelectrons as determined from the Atoms in this set */
        void SetDefaults_(void);

    public:
        typedef Atom::CoordType CoordType;
        typedef AtomSet::value_type value_type;
        typedef AtomSet::const_iterator const_iterator;

        /*! A function or functor that selects Atoms from this Molecule
         * 
         * Takes a `const Atom &` as an argument and returns true if it
         * should be included in a new Molecule
         */
        typedef AtomSet::SelectorFunc SelectorFunc;

        /*! A function or functor that transforms Atoms from this Molecule
         * 
         * Takes a `const Atom &` as an argument and returns a new Atom
         */
        typedef AtomSet::TransformerFunc TransformerFunc;


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


        /*! \name General Molecule properties
         */ 
        ///@{ 

        /*! \brief Return the number of Atoms in the Molecule (NOT the Universe) */
        int NAtoms(void) const;

        /*! \brief Get the charge of the molecule */
        double GetCharge(void) const;

        /*! \brief Get the charge of the molecule as determined by summing
         *         the charges on all atoms */
        double GetSumCharge(void) const;

        /*! \brief Set the charge of the molecule */
        void SetCharge(double charge);

        /*! \brief Get the number of electrons in the molecule */
        double GetNElectrons(void) const;

        /*! \brief Get the number of electrons in the molecule as
         *         determined by summing the number of electrons for all atoms*/
        double GetSumNElectrons(void) const;

        /*! \brief Set the number of electrons in the molecule */
        void SetNElectrons(double nelectrons);

        /*! \brief Get the multiplicity of the Molecule */
        double GetMultiplicity(void) const;

        /*! \brief Set the multiplicity of the Molecule */
        void SetMultiplicity(double m);


        /*! \brief Does this molecule have an atom with the given index
         * 
         * \note The index refers to the unique index of the atom (typically
         *       input ordering), NOT its order within this molecule.
         */
        bool HasAtom(size_t atomidx) const;


        /*! \brief Obtain an atom with the given index
         * 
         * \note The index refers to the unique index of the atom (typically
         *       input ordering), NOT its order within this molecule.
         *
         * \throw bpmodule::exception::SystemException If an atom with the given index
         *        is not part of this Molecule
         */
        Atom GetAtom(size_t atomidx) const;

        ///@}


        /* Printing
         * \todo What to do about printing
         */
        std::string ToString(void) const;


        /*! \name Iteration over atoms
         */ 
        ///@{


        /*! \brief Return an iterator to the beginning of this molecule (first atom) */
        const_iterator begin(void) const { return atoms_.begin(); }

        /*! \brief Return an iterator to the end of this molecule (just past the last atom) */
        const_iterator end(void) const { return atoms_.end(); }

        ///@}


        /*! \name Calculated properties */
        ///@{

        /*! \brief Find the center of mass
         * 
         * Uses atomic masses (abundance-weighted isotope masses)
         */
        math::Point CenterOfMass(void) const;

        /*! \brief Find the center of nuclear charge */
        math::Point CenterOfNuclearCharge(void) const;

        ///@}


        /*! \name Set operations
         */
        ///@{ 

        /*! \brief Insert an atom into this molecule
         * 
         * The Atom must already be a part of this Molecule's universe
         *
         * \todo Exceptions from MathSet
         */
        void Insert(const Atom & atom);

        /*! \brief Return a subset of atoms in the molecule */
        Molecule Partition(SelectorFunc selector) const;

        /*! \brief Return all atoms that are in the universe but not in this molecule */
        Molecule Complement(void) const;

        /*! \brief Return atoms that are in both this molecule and \p rhs */
        Molecule Intersection(const Molecule & rhs) const;

        /*! \brief Return atoms that are in either this molecule and \p rhs */
        Molecule Union(const Molecule & rhs) const;

        /*! \brief Return atoms that are in this molecule but not in \p rhs */
        Molecule Difference(const Molecule & rhs) const;
        

        ///@}


        /*! \name Basis Set-related functions
         */
        ///@{

        /*! \brief See if this molecule has a particular basis set
         * 
         * Returns false if no atoms have the basis set
         */ 
        bool HasBasisSet(const std::string & basislabel) const;


        /*! \brief Obtain a basis set with the given label
         */
        basisset::BasisSet GetBasisSet(const std::string & basislabel) const;

        ///@}


        /*! \name General Manipulations
         * 
         * These all return a new Molecule
         */
        ///@{

        /*! \brief Perform a generic transformation to atoms in the molecule */
        Molecule Transform(TransformerFunc transformer) const;


        /*! \brief Translate the molecule
         * 
         * \tparam VectorType A type corresponding to a VectorConcept
         */ 
        template<typename VectorType>
        Molecule Translate(const VectorType & vec) const
        {
            return Transform(std::bind(math::TranslatePoint_Copy<Atom, VectorType>, std::placeholders::_1, vec));
        }

        /*! \brief Rotate the molecule
         * 
         * \tparam VectorType A type corresponding to a MatrixConcept
         */ 
        template<typename MatrixType>
        Molecule Rotate(const MatrixType & mat) const
        {
            return Transform(std::bind(math::RotatePoint_Copy<Atom, MatrixType>, std::placeholders::_1, mat));
        }

        ///@}


};



} // close namespace system
} // close namespace bpmodule


#endif
