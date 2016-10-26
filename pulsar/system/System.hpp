/*\file
 *
 * \brief Declares the System class as well as a host of functions for
 * performing common tasks associated with the system.
 */


#ifndef PULSAR_GUARD_SYSTEM__SYSTEM_HPP_
#define PULSAR_GUARD_SYSTEM__SYSTEM_HPP_

#include <unordered_map>
#include <unordered_set>
#include "pulsar/system/Atom.hpp"
#include "pulsar/system/Space.hpp"
#include "pulsar/system/BasisSet.hpp"
#include "pulsar/math/MathSet.hpp"
#include "pulsar/math/PointManipulation.hpp"
#include "bphash/Hasher.hpp"
#include "pulsar/util/Hash.hpp"


// Instantiated in the cpp file
extern template class pulsar::math::Universe<pulsar::system::Atom>;
extern template class pulsar::math::MathSet<pulsar::system::Atom>;



namespace pulsar{
namespace system {


/*! \brief All atoms that may be used in a group of Systems */
typedef math::Universe<Atom> AtomSetUniverse;

/*! \brief A collection of Atoms and the space around them
 * 
 *  This class is described in detail in the page 
 *  [How to Make and Use Systems](@ref systems).  The main
 *  take away from a developer standpoint is that only the set of
 *  atoms is private.  This is because adding/removing atoms modifies the
 *  properties of the system as a whole (charge,mass, etc.).  Consequentially,
 *  we need thin wrappers around the member functions of the MathSet class.
 */
class System {
private:    
    using AtomSet=math::MathSet<Atom>;//!< Type of atom storage container
    AtomSet atoms_;//!< Actual set of atoms

    /*! \brief Construct a system given a universe
     *
     * The universe will be shared with the data that was passed in
     *
     * \param [in] universe The universe this system should use 
     * \param [in] fill If true system contains all the elements of the universe
     */
    explicit System(std::shared_ptr<const AtomSetUniverse> universe, bool fill);

    
    /* \brief Sets charge, multiplicity, and nelectrons as determined from the Atoms in this set */
    void SetDefaults_(void);


    //! \name Serialization and Hashing
    ///@{

    DECLARE_SERIALIZATION_FRIENDS
    BPHASH_DECLARE_HASHING_FRIENDS

    template<class Archive>
    void serialize(Archive & ar)
    {
        ar(atoms_, mass, charge, multiplicity, nelectrons);
    }

    void hash(bphash::Hasher & h) const;
    
    ///@}

public:
    
    using value_type=AtomSet::value_type;///< The type of an atom
    using const_iterator=AtomSet::const_iterator;///< Iterator over atoms
    using SelectorFunc=AtomSet::SelectorFunc;///<\copydoc MathSet::SelectorFunc
    using TransformerFunc=AtomSet::TransformerFunc;///<\copydoc MathSet::TransformerFunc

    double mass;   //!<Total mass of the system
    double charge; //!< Total charge on this system
    double multiplicity; //!< Total multiplicity of the system
    double nelectrons; //!< Total number of electrons in the system 
    Space space;//!< The space this system lives in
    
    ///Copies the atoms in \p universe and add the atoms if \p fill is true
    explicit System(const AtomSetUniverse& universe, bool fill);
    ///Takes ownership of atoms in \p universe and adds them if \p fill is true
    explicit System(AtomSetUniverse&& universe,bool fill);

    /*! \brief For serialization only
     * 
     * \warning NOT FOR USE OUTSIDE OF SERIALIZATION
     * \todo Replace if cereal fixes this
     */
    System() = default;

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

    size_t size(void) const;///< Returns atoms in system NOT universe

    double get_sum_charge(void) const;///< Returns sum of charg on each atom

    double get_sum_n_electrons(void) const;///< Returns sum of each atoms' electrons
    
    double get_sum_mass(void) const;///< Returns the sum of the atoms' masses

    bool count(const Atom& AnAtom)const;///< Returns true if atom is in system

    bool compare_info(const System & RHS)const;///<Compares data not in Space or atoms

    bphash::HashValue my_hash(void) const;///< Returns hash of System

    ///@}

    std::ostream& print(std::ostream & os) const;

    ///@{

    /*! \brief Return an iterator to the beginning of this system (first atom) */
    const_iterator begin(void) const{return atoms_.begin();}

    /*! \brief Return an iterator to the end of this system (just past the last atom) */
    const_iterator end(void) const{return atoms_.end();}

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
    System & insert(const Atom & atom);
    
    ///For compatability with STL algorithms
    template<typename T>
    System & insert(const T&,const Atom& atom){
        return this->insert(atom);
    }
    
    ///Range based insertion
    template<typename Begin_t,typename End_t>
    System & insert(Begin_t begin,End_t end){
        while(begin!=end){
            this->insert(*begin);
            ++begin;
        }
        return *this;
    }

    /*! \brief Insert an atom into this system
     * 
     * The Atom must already be a part of this System's universe
     *
     * \todo Exceptions from MathSet
     */
    System & insert(Atom && atom);
    
    ///For compatability with STL algorithms
    template<typename T>
    System & insert(const T&,Atom&& atom){
        return this->insert(std::move(atom));
    }
    
    ///This becomes union of this and RHS
    System & union_assign(const System& RHS);
    System set_union(const System& RHS) const;///<Returns union of this and RHS
    ///This becomes intersection of this and RHS
    System & intersection_assign(const System& RHS);
    ///Returns intersection of this and RHS
    System intersection(const System& RHS) const;
    ///This becomes difference of this and RHS
    System & difference_assign(const System& RHS);
    ///Returns the set difference of this and RHS
    System difference(const System& RHS) const;
    
    /*! \brief Return all atoms that are in the universe but not in this system */
    System complement(void) const;

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
    bool is_proper_subset_of(const System& RHS) const;


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
    bool is_subset_of(const System& RHS) const;


    /** \brief Returns true if this is a proper superset of other
     * 
     *  This is a proper superset of other, iff other is a proper subset
     *  of this.
     * 
     *  \param[in] RHS Set to compare to
     *  \return True if this is a proper superset of other
     *
     */
    bool is_proper_superset_of(const System& RHS) const;


    /** \brief Returns true if this is a superset of other
     * 
     *  This is a superset of other iff other is a subset of this
     * 
     *  \param[in] RHS Set to compare to
     *  \return true if this is a superset of other
     *  
     */
    bool is_superset_of(const System& RHS) const;

    /** \brief Returns true if this set equals other
     * 
     *   We define equality as having the same atoms, the same charge,
     *   the same multiplicity, the same mass, and the same number of electrons.
     *   We override MathSet's requirement for the same universes.
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
    
    /*! \brief Return a subset of atoms in the system */
    System partition(SelectorFunc selector) const;
    
    ///@}


    /*! \name Basis Set-related functions
     */
    ///@{

    /*! \brief See if this system has a particular basis set
     * 
     * Returns false if no atoms have the basis set
     */
    bool has_basis_set(const std::string & basislabel) const;


    /*! \brief Obtain a basis set with the given label
     */
    BasisSet get_basis_set(const std::string & basislabel) const;

    ///@}


    /*! \name General Manipulations
     * 
     * These all return a new System
     */
    ///@{

    /*! \brief Perform a generic transformation to atoms in the system */
    System transform(TransformerFunc Transformer) const;

    /*! \brief Obtain the universe in use by this system
     */
    std::shared_ptr<const AtomSetUniverse> get_universe(void) const;

    /*! \brief Obtain the atoms from this system as a new universe
     * 
     * The universe is not linked to this system in any way. Other information
     * (such as symmetry, etc) is obviously not copied.
     */
    AtomSetUniverse as_universe(void) const;

    ///@}
};

/*! \brief Finds the center of mass of \p sys
 * 
 * Uses atomic masses (abundance-weighted isotope masses)
 */
math::Point center_of_mass(const System& sys);

/*! \brief Finds the center of nuclear charge of \p sys*/
math::Point center_of_nuclear_charge(const System& sys);

/*! \brief translate the system
 * 
 * \tparam VectorType A type corresponding to a VectorConcept
 */
template<typename VectorType>
System translate(const System& Sys,const VectorType & vec)
{
    return Sys.transform(std::bind(math::translate_point_copy<Atom, VectorType>, 
            std::placeholders::_1, vec));
}

/*! \brief rotates the system point by point
 * 
 *  Given a rotation matrix R, this function takes the point from the
 *  column space to the row space.
 * 
 * \tparam VectorType A type corresponding to a MatrixConcept
 */
template<typename MatrixType>
System rotate(const System& Sys,const MatrixType & mat)
{
    return Sys.transform(std::bind(math::rotate_point_copy<Atom, MatrixType>, 
            std::placeholders::_1, mat));
}

///Type of a distance matrix
typedef std::unordered_map<std::pair<Atom,Atom>,double> DistMat_t;

///Returns the distance between each pair of atoms in sys
DistMat_t get_distance(const System& sys);

///Type of a connectivity table
typedef std::unordered_map<Atom, std::unordered_set<Atom>> Conn_t;

///Returns connectivity data of sys, using covalent radii.  Atoms are considered
///bonded if Tolerance*(sum of covRaddii) is greater than distance
Conn_t get_connectivity(const System& sys, double Tolerance = 1.20);

/** \brief Given a molecule that has been translated to the center of mass
 *         this function computes the inertia tensor for that system
 * 
 *  The moment of inertia of a rigid rotating body comprised of \f$N\f$
 *  particles rotating about some axis is given by:
 *  \f[
 *     I=\sum_i^N m_iR_i^2,
 *  \f]
 *  where \f$m_i\f$ is the mass of the \f$i\f$-th particle and \f$R_i\f$ is
 *  the distance to the axis of rotation.  In most cases we instead know the
 *  distance to the origin, \f$r_i\f$.  If we assume that the axis of rotation passes
 *  through the origin we have a right triangle formed from \f$R_i\f$, 
 *  \f$r_i\f$, and a unit vector in \f$n\f$ parallel with the rotation axis.
 *  Therefore we know:
 *  \f{align}{
 *    I=&\sum_i^N m_i\left[\left|r_i\right|\sin\left(\theta\right)\right]^2
 *     =&\sum_i^N m_i\left(r_i \times n\right)^2
 *  \f}
 *  where \f$\theta\f$ is the angle between \f$r_i\f$ and \f$n\f$.  In general,
 *  \f$n\f$ will not be parallel to any of the axes for which \f$r_i\f$ is
 *  defined.  We can get its projection onto those axes via the dot-product
 *  and consequentially, 
 *  \f$n=\left[\cos\left(\alpha\right),\cos\left(\beta\right),\cos\left(\gamma\right)\right]\f$,
 *  where \f$\alpha,\beta\f$, and \f$\gamma\f$ are the angles between \f$n\f$ and
 *  the x,y, and z axes in the \f$r_i\f$ frame respectively.  After a little
 *  math we have:
 *  \f[
 *    r_i\times n=\left[r_{iy}\cos\left(\gamma\right)-
 *                      r_{iz}\cos\left(\beta\right),
 *                      r_{iz}\cos\left(\alpha\right)-
 *                      r_{ix}\cos\left(\gamma\right),
 *                      r_{ix}\cos\left(\beta\right)-
 *                      r_{iy}\cos\left(\alpha\right)\right]
 *  \f]
 *  Squaring this we get:
 *  \f[
 *    \left(r_i\times n\right)^2=r^2_{iy}\cos^2\left(\gamma\right)-
 *      2r_{iy}r_{iz}\cos\left(\beta\right)\cos\left(\gamma\right)+
 *      r^2_{iz}\cos^2\left(\beta\right)+
 *      r^2_{iz}\cos^2\left(\alpha\right)-
 *      r_{ix}r_{iz}\cos\left(\alpha\right)\cos\left(\gamma\right)+
 *      r^2_{ix}\cos^2\left(\gamma\right)+
 *      r^2_{ix}\cos^2\left(\beta\right)-
 *      r_{ix}r_{iy}\cos\left(\alpha\right)\cos\left(\beta\right)+
 *      r^2_{iy}\cos^2\left(\alpha\right)
 *  \f]
 *  Upon collecting \f$\cos\f$ terms:
 *  \f[
 *    \left(r_i\times n\right)^2=
 *      \left(r^2_{ix}+r^2_{iy}\right)\cos^2\left(\gamma\right)-
 *      2r_{iy}r_{iz}\cos\left(\beta\right)\cos\left(\gamma\right)+
 *      \left(r^2_{ix}+r^2_{iz}\right)\cos^2\left(\beta\right)-
 *      2r_{ix}r_{iz}\cos\left(\alpha\right)\cos\left(\gamma\right)-
 *      2r_{ix}r_{iy}\cos\left(\alpha\right)\cos\left(\beta\right)+
 *      \left(r^2_{iy}+r^2_{iz}\right)\cos^2\left(\alpha\right)
 *  \f]
 *  Defining:
 *  \f{eqnarray}{
 *     I_{xx}=&\sum_i^N m_i\left(r^2_{iy}+r^2_{iz}\right)\\
 *     I_{xy}=&-\sum_i^N m_ir_{ix}r_{iy}\\
 *     I_{xz}=&-\sum_i^N m_ir_{ix}r_[iz}\\
 *     I_{yy}=&\sum_i^N m_i\left(r^2_{ix}+r^2_{iz}\right)\\
 *     I_{yz}=&-\sum_i^N m_ir_{iy}r_{iz}\\
 *     I_{zz}=&\sum_i^Nm_i\left(r^2_{ix}+r^2_{iy}\right)
 *  \f}
 *  We can write:
 *  \f[
 *    I=\sum_i^Nm_i\left(r_i\times n\right)^2=
 *      I_{xx}\cos^2\left(\alpha\right)+
 *      I_{yy}\cos^2\left(\beta\right)+
 *      I_{zz}\cos^2\left(\gamma\right)+
 *      2I_{xy}\cos\left(\alpha\right)\cos\left(\beta\right)+ 
 *      2I_{xz}\cos\left(\alpha\right)\cos\left(\gamma\right)
 *      2I_{yz}\cos\left(\beta\right)\cos\left(\gamma\right)=
 *      n^T \mathbf{I} n,
 *  \f]
 *   where \f$\mathbf{I}\f$ is the inertia tensor.  Consequentially the
 *   inertial tensor is not simply the second moments of the mass distribution
 *   when their coordinates are expressed in terms of their initial coordinate
 *   system, but:
 *   \f[
 *      I_{pq}=\sum_i^Nm_ir^2_{i}\delta_{pq}-m_ir_{ip}r_{iq}
 *   \f]
 */
std::array<double,9> inertia_tensor(const System& Mol);


///Allows a system to be printed with stream operators
inline std::ostream& operator<<(std::ostream& os, const System& Mol)
{
    return Mol.print(os);
}

//! A map of systems (fragments, etc), MIM uses fact that this is ordered
typedef std::map<std::string, System> SystemMap;

///Helper function that converts a system to Angstroms (for printing/debugging)
System system_to_angstroms(const System& sys);

///Operator overload of an assignment operation
#define ASSIGN_OP(fxn,fxn2call)\
inline System& fxn(System& lhs,const System& rhs){return lhs.fxn2call(rhs);}

///Operator overload of a const operation
#define SYS_OP(rv,fxn,fxn2call)\
inline rv fxn(const System& lhs,const System& rhs){return lhs.fxn2call(rhs);}

ASSIGN_OP(operator+=,union_assign)
ASSIGN_OP(operator/=,intersection_assign)
ASSIGN_OP(operator-=,difference_assign)
SYS_OP(System,operator+,set_union)
SYS_OP(System,operator/,intersection)
SYS_OP(System,operator-,difference)
SYS_OP(bool,operator<=,is_subset_of)
SYS_OP(bool,operator<,is_proper_subset_of)
SYS_OP(bool,operator>=,is_superset_of)
SYS_OP(bool,operator>,is_proper_superset_of)

#undef SYS_OP
#undef ASSIGN_OP

} // close namespace system
} // close namespace pulsar


#endif
