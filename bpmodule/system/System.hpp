/*\file
 *
 * \brief The system class (header)
 */


#ifndef BPMODULE_GUARD_SYSTEM__SYSTEM_HPP_
#define BPMODULE_GUARD_SYSTEM__SYSTEM_HPP_

#include <unordered_map>
#include <unordered_set>
#include "bpmodule/system/Atom.hpp"
#include "bpmodule/math/MathSet.hpp"
#include "bpmodule/math/PointManipulation.hpp"

// Instantiated in the cpp file
extern template class bpmodule::math::Universe<bpmodule::system::Atom>;
extern template class bpmodule::math::MathSet<bpmodule::system::Atom>;


// forward declare
namespace bpmodule {
namespace system {
class BasisSet;
}
}


namespace bpmodule {
namespace system {


/*! \brief All atoms that may be used in a group of Systems */
typedef math::Universe<Atom> AtomSetUniverse;

/*! \brief A collection of Atoms
 *
 * Once atoms are placed in a System (or AtomSetUniverse), they
 * cannot be changed in any way. This allows for consistency when
 * atoms are shared among may System objects (via the universe).
 * Changing the Atoms themselves require making copies of the Atom.
 * This can be accomplished easily through the System::Transform function,
 * which returns a new System containing a new universe. This new system
 * is completely divorced from the original and does not share any resources
 * with it.
 *
 * The only thing modifiable about a non-const System is which Atoms (from
 * of its universe) it contains.
 * Assignment and copy construction will result in Systems sharing the
 * universe, but with separate lists of atoms they contain.
 *
 * This System object will behave like a MathSet, allowing for
 * unions, intersections, complements, etc.
 */
class System {
private:
    //! How atoms are actually stored in the System
    typedef math::MathSet<Atom> AtomSet;

    //! Actual storage of all the atoms
    AtomSet atoms_;

    // For use from transformations, etc
    System(const AtomSet & atoms);

    double charge_; //!< Total charge on this system
    double multiplicity_; //!< Total multiplicity of the system
    double nelectrons_; //!< Total number of electrons in the system 

    /* \brief Sets charge, multiplicity, and nelectrons as determined from the Atoms in this set */
    void SetDefaults_(void);

public:
    typedef AtomSet::value_type value_type;
    typedef AtomSet::const_iterator const_iterator;

    /*! A function or functor that selects Atoms from this System
     * 
     * Takes a `const Atom &` as an argument and returns true if it
     * should be included in a new System
     */
    typedef AtomSet::SelectorFunc SelectorFunc;

    /*! A function or functor that transforms Atoms from this System
     * 
     * Takes a `const Atom &` as an argument and returns a new Atom
     */
    typedef AtomSet::TransformerFunc TransformerFunc;


    /*! \brief Construct a system given a universe
     *
     * \param [in] universe The universe this system should use 
     * \param [in] fill Make this system contain all the elements of the universe
     */
    System(std::shared_ptr<const AtomSetUniverse> universe, bool fill);
    System(const AtomSetUniverse& universe, bool fill);

    // compiler generated ok
    // Copies will share storage in the AtomSet, but have their
    // own sets of contained atoms
    System(const System & rhs) = default;
    System(System && rhs) = default;
    System & operator=(const System & rhs) = default;
    System & operator=(System && rhs) = default;


    /*! \name General System properties
     */
    ///@{ 

    /*! \brief Return the number of Atoms in the System (NOT the Universe) */
    int NAtoms(void) const;

    /*! \brief Get the charge of the system */
    double GetCharge(void) const;

    /*! \brief Get the charge of the system as determined by summing
     *         the charges on all atoms */
    double GetSumCharge(void) const;

    /*! \brief Set the charge of the system */
    void SetCharge(double charge);

    /*! \brief Get the number of electrons in the system */
    double GetNElectrons(void) const;

    /*! \brief Get the number of electrons in the system as
     *         determined by summing the number of electrons for all atoms*/
    double GetSumNElectrons(void) const;

    /*! \brief Set the number of electrons in the system */
    void SetNElectrons(double nelectrons);

    /*! \brief Get the multiplicity of the System */
    double GetMultiplicity(void) const;

    /*! \brief Set the multiplicity of the System */
    void SetMultiplicity(double m);


    /*! \brief Does this system have an atom with the given index
     * 
     * \note The index refers to the unique index of the atom (typically
     *       input ordering), NOT its order within this system.
     */
    //bool HasAtom(size_t atomidx) const;
    
    ///Returns true if the system contains the atom
    bool HasAtom(const Atom& AnAtom)const;


    /*! \brief Obtain an atom with the given index
     * 
     * \note The index refers to the unique index of the atom (typically
     *       input ordering), NOT its order within this system.
     *
     * \throw bpmodule::exception::SystemException If an atom with the given index
     *        is not part of this System
     */
    Atom GetAtom(size_t atomidx) const;

    ///@}


    /* Printing
     * \todo What to do about printing
     */
    std::string ToString(void) const;


    void Print(std::ostream & os) const;


    /*! \name Iteration over atoms
     *  
     *  As long as a system has not been modified (i.e. an atom added or
     *  deleted) the iteration order is defined.  The atom that you get first
     *  will always be first, the atom you get second, will be second, etc.
     */
    ///@{

    /*! \brief Return an iterator to the beginning of this system (first atom) */
    const_iterator begin(void) const
    {
        return atoms_.begin();
    }

    /*! \brief Return an iterator to the end of this system (just past the last atom) */
    const_iterator end(void) const
    {
        return atoms_.end();
    }

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

    /*! \brief Insert an atom into this system
     * 
     * The Atom must already be a part of this System's universe
     *
     * \todo Exceptions from MathSet
     */
    void Insert(const Atom & atom);

    /*! \brief Return a subset of atoms in the system */
    System Partition(SelectorFunc selector) const;

    /*! \brief Return all atoms that are in the universe but not in this system */
    System Complement(void) const;

    /*! \brief Return atoms that are in both this system and \p rhs */
    System Intersection(const System & rhs) const;
    
    /** \brief Makes this the intersection of this and \p rhs*/
    System& operator/=(const System& rhs);
    /** \brief Returns the intersection of this and \p rhs*/
    System operator/(const System& rhs)const{
        return Intersection(rhs);
    }

    /*! \brief Return atoms that are in either this system and \p rhs */
    System Union(const System & rhs) const;
    /** \brief Makes this the union of this and \p rhs*/
    System& operator+=(const System& rhs);
    /** \brief Returns the union of this and \p rhs*/
    System operator+(const System& rhs)const{
        return Union(rhs);
    }

    /*! \brief Return atoms that are in this system but not in \p rhs */
    System Difference(const System & rhs) const;
    /** \brief Makes this the set difference of this and \p rhs*/
    System& operator-=(const System& rhs);
    /** \brief Returns the set difference of this and \p rhs*/
    System operator-(const System& rhs)const{
        return Difference(rhs);
    }
    ///@}

    /** \name System comparers
     * 
     *  Potential Gotcha:  Always compare for what you care about.  For example,
     *  if you want to know if system A is a subset of another system B don't
     *  negate B<A because "B is not a subset of A" does not imply that
     *  "A is a subset of B" (they may have no relation)
     *  
     */
    ///@{

    /** \brief Returns true if this set equals other
     * 
     *   We define equality as having the same atoms, the same charge,
     *   the same multiplicity, and the same number of electrons
     * 
     *   \param[in] RHS The other System to compare to
     *   \return True if this set equals other
     */
    bool operator==(const System& RHS)const;

    /** \brief Returns true if this does not equal other
     * 
     *   Literally just negates the result of operator==()
     * 
     *   \param[in] RHS The other System
     *   \return True if the sets are different
     * 
     */
    bool operator!=(const System& RHS)const
    {
        return !(*this == RHS);
    }

    /** \brief Returns true if this is a proper subset of other
     * 
     *   This is a proper subset of RHS if the atoms are a proper subset
     *   of other.  Charge, multiplicity, and number of electrons are
     *   ignored.
     * 
     *   \param[in] RHS The system to compare to
     *   \return True if this is a proper subset of other
     *
     */
    bool operator<(const System& RHS)const;

    /** \brief Returns true if this is a subset of other
     * 
     *   This is a subset of RHS if the atoms are a subset
     *   of other.  Charge, multiplicity, and number of electrons are
     *   ignored.
     * 
     *   \param[in] RHS The set to comapre to
     *   \return True if this is a subset of other
     *
     */
    bool operator<=(const System& RHS)const;

    /** \brief Returns true if this is a proper superset of other
     * 
     *  This is a proper superset of other, iff other is a proper subset
     *  of this.
     * 
     *  \param[in] RHS Set to compare to
     *  \return True if this is a proper superset of other
     *
     */
    bool operator>(const System& RHS)const
    {
        return RHS<*this;
    }

    /** \brief Returns true if this is a superset of other
     * 
     *  This is a superset of other iff other is a subset of this
     * 
     *  \param[in] RHS Set to compare to
     *  \return true if this is a superset of other
     *  
     */
    bool operator>=(const System& RHS)const
    {
        return RHS <= *this;
    }
    ///@}


    /*! \name Basis Set-related functions
     */
    ///@{

    /*! \brief See if this system has a particular basis set
     * 
     * Returns false if no atoms have the basis set
     */
    bool HasBasisSet(const std::string & basislabel) const;


    /*! \brief Obtain a basis set with the given label
     */
    BasisSet GetBasisSet(const std::string & basislabel) const;

    ///@}


    /*! \name General Manipulations
     * 
     * These all return a new System
     */
    ///@{

    /*! \brief Perform a generic transformation to atoms in the system */
    System Transform(TransformerFunc transformer) const;

    /*! \brief Translate the system
     * 
     * \tparam VectorType A type corresponding to a VectorConcept
     */
    template<typename VectorType>
    System Translate(const VectorType & vec) const
    {
        return Transform(std::bind(math::TranslatePoint_Copy<Atom, VectorType>, std::placeholders::_1, vec));
    }

    /*! \brief Rotate the system
     * 
     * \tparam VectorType A type corresponding to a MatrixConcept
     */
    template<typename MatrixType>
    System Rotate(const MatrixType & mat) const
    {
        return Transform(std::bind(math::RotatePoint_Copy<Atom, MatrixType>, std::placeholders::_1, mat));
    }

    ///@}
};

///Returns the distance between each pair of atoms in sys
std::vector<double> GetDistance(const System& sys);

///Returns connectivity data of sys, using covalent radii.  Atoms are considered
///bonded if Tolerance*sum of covRaddii is greater than distance
std::unordered_map<Atom, std::unordered_set<Atom>> GetConns(const System& sys, double Tolerance = 1.20);

inline std::ostream& operator<<(std::ostream& os, const System& Mol)
{
    return os << Mol.ToString();
}

//! A map of systems (fragments, etc), MIM uses fact that this is ordered
typedef std::map<std::string, System> SystemMap;

} // close namespace system
} // close namespace bpmodule


#endif
