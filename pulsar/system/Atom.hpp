/*\file
 *
 * \brief Atoms and atomic centers (header)
*/


#ifndef PULSAR_GUARD_SYSTEM__ATOM_HPP_
#define PULSAR_GUARD_SYSTEM__ATOM_HPP_

#include <iostream>
#include <map>

#include "pulsar/system/CoordType.hpp"
#include "pulsar/system/BasisInfo.hpp"
#include "pulsar/util/Serialization.hpp"
#include "bphash/Hasher.hpp"
#include "bphash/types/vector.hpp"
#include "bphash/types/string.hpp"
#include "bphash/types/map.hpp"


namespace pulsar {

/*! \brief A center in a system
 *
 * Generally, Atoms would be created by one of the create_atom
 * free functions, which fill in much of the information with defaults
 * given a Z number (and optionally an isotope number).
 *
 * Many values which are traditionally integers are instead represented
 * as doubles, which allows for fractional occupation.
 *
 * Data is stored within the Atom object, therefore copying will not
 * create aliases.
 *
 * Follows PointConcept via derivation from Point
 */
struct Atom : public Point
{
    public:

        int Z;                 //!< Atomic Z number (as integer. Also stored as a (double) weight)
        int isotope;           //!< Isotope number

        double mass;           //!< Atomic mass (abundance-weighted isotope masses)
        double isotope_mass;   //!< Mass of the selected isotope
        double charge;         //!< Charge on the center
        double multiplicity;   //!< Electronic multiplicity
        double nelectrons;     //!< Number of assigned electrons
        double cov_radius;     //!< The covalent radius
        double vdw_radius;     //!< The van der waals radius

        std::map<std::string, BasisInfo> basis_sets; //!< Map of tag to basis functions associated with this atom/center


        /*! \brief Constructor
         * 
         * \todo C++17 may allow for aggregate initialization
         */
        Atom(CoordType xyz, int Z, int isotope, double mass,
             double isotope_mass, double charge, double multiplicity,
             double nelectrons,double cov_radius,double vdw_radius);


        /*! \brief For serialization only
         * 
         * \warning NOT FOR USE OUTSIDE OF SERIALIZATION
         * \todo Replace if cereal fixes this
         */
        Atom() = default;

        Atom(const Atom &)             = default;///<Deep copy
        Atom & operator=(const Atom &) = default;///<Deep copy
        Atom(Atom &&)                  = default;
        Atom & operator=(Atom &&)      = default;


        /* \brief Equality comparison
         *
         * This compares all components individually
         */
        bool operator==(const Atom & rhs) const;


        /* \brief Inequality comparison
         *
         * This compares all components individually
         */
        bool operator!=(const Atom & rhs) const;


        /*! \name printing */
        ///@{

        void print(std::ostream & os) const;

        ///@}
        
        ///Hashes all attributes of the class and returns the value
        bphash::HashValue my_hash(void) const;


    private:

        //! \name Serialization and Hashing
        ///@{

        DECLARE_SERIALIZATION_FRIENDS
        BPHASH_DECLARE_HASHING_FRIENDS


        template<class Archive>
        void serialize(Archive & ar)
        {
            // we aren't serializing the base class, so we do this manually
            ar(cereal::base_class<Point>(this), Z, isotope,
                                                mass, isotope_mass,
                                                charge, multiplicity,
                                                nelectrons, cov_radius,
                                                vdw_radius, basis_sets);
        }

        void hash(bphash::Hasher & h) const;

        ///@}


};

//! \todo What to do about printing
std::ostream& operator<<(std::ostream& os,const Atom& A);




/* \brief Create an atom given an ID, coordinates, and atomic number
 *
 * The rest of the data is filled in automatically
 */
Atom create_atom(CoordType xyz, int Z);


/*! \copydoc create_atom(CoordType xyz, int Z) */
//Atom create_atom(double x, double y, double z, int Z);



/* \brief Create an atom given an ID, coordinates, atomic number, and isotope number
 *
 * The rest of the data is filled in automatically
 */
Atom create_atom(CoordType xyz, int Z, int isonum);


/*! \copydoc create_atom(CoordType xyz, int Z, int isonum) */ 
//Atom create_atom(double x, double y, double z, int Z, int isonum);


///@{ \name Ghost Atom Functions
/** When making/checking for ghost atoms please use these functions to ensure
 *  consistent definitions of ghost atoms.  A ghost atom has the same basis
 *  functions as the atom it is replacing, but no other properties, i.e.
 *  a charge of 0, a mass of 0, a multiplicity of 0, etc.  Ghost atoms
 *  must be made from atoms that have basis functions already because the
 *  Z information will be lost.  In practice this poses no problems for
 *  any existing BSSE method.
 */

///Makes a ghost atom with the same coords and basis funtions as another atom
Atom make_ghost_atom(const Atom & atom);

///Returns true if AtomI is a ghost atom
bool is_ghost_atom(const Atom & atom);
///@}


///@{\name Point Charge Functions
/** When making/checking for point charges please use these functions to ensure
 *  consistent definitions of point charge.  A point charge has a charge, but
 *  no other properties.
 */
///Makes a charge from a charge and coordinates
Atom make_point_charge(const CoordType & xyz, double charge);

///Copies AtomI into a point charge
Atom make_point_charge(const Atom & atom,double charge);

///Makes a point charge from a charge and coordinates
//Atom make_point_charge(double x, double y, double z, double charge);

///Returns true if AtomI is a point charge
bool is_point_charge(const Atom& AtomI);
///@}

///@{\name Dummy Atom Functions
/** When making/checking for point charges please use these functions to ensure
 *  consistent definitions of point charge.  A point charge has a charge, but
 *  no other properties.
 */
///Makes a dummy atom from coordinates
Atom make_dummy_atom(const CoordType & xyz);

///Copies AtomI into a dummy atom
Atom make_dummy_atom(const Atom & atom);

///Makes a dummy atom from component coordinates
//Atom make_dummy_atom(double x, double y, double z);

///Returns true if AtomI is a dummy atom
bool is_dummy_atom(const Atom & atom);
///@}

} // close namespace pulsar

///Allows atoms to be hashed
namespace std{
    template<> struct hash<pulsar::Atom>
    {
        size_t operator()(const pulsar::Atom& atom)const{
            return bphash::convert_hash<size_t>(atom.my_hash());
        }
    };
}

#endif

