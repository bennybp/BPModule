/*\file
 *
 * \brief Atoms and atomic centers (source)
*/


#include "pulsar/system/Atom.hpp"
#include "pulsar/system/AtomicInfo.hpp"
#include "pulsar/math/Cast.hpp"
#include "pulsar/output/Output.hpp"

#include "bphash/types/string.hpp"
#include "bphash/types/set.hpp"
#include "bphash/types/map.hpp"
#include "bphash/types/vector.hpp"

namespace pulsar{



/////////////////////////////////////////
// BasisInfo
/////////////////////////////////////////

Atom::Atom(CoordType xyz_, int Z_, int isotope_, double mass_,
           double isotope_mass_, double charge_, double multiplicity_,
           double nelectrons_,double cov_radius_, double vdw_radius_)
    : Point(xyz_), Z(Z_), isotope(isotope_), mass(mass_),
      isotope_mass(isotope_mass_), charge(charge_), multiplicity(multiplicity_),
      nelectrons(nelectrons_), cov_radius(cov_radius_), vdw_radius(vdw_radius_)
{ }

bool Atom::operator==(const Atom & rhs) const
{
    PRAGMA_WARNING_PUSH
    PRAGMA_WARNING_IGNORE_FP_EQUALITY

    // order by the parts that are most likely to be different, since
    //   this should short-circuit on the first false comparison
    return Z == rhs.Z &&
           static_cast<const Point>(*this) == static_cast<const Point>(rhs) &&
           basis_sets == rhs.basis_sets &&
           isotope == rhs.isotope &&
           mass == rhs.mass &&
           isotope_mass == rhs.isotope_mass &&
           charge == rhs.charge &&
           multiplicity == rhs.multiplicity &&
           nelectrons == rhs.nelectrons &&
           cov_radius == rhs.cov_radius &&
           vdw_radius == rhs.vdw_radius
           ;
             
    PRAGMA_WARNING_POP
}

bool Atom::operator!=(const Atom & rhs) const
{
    return !((*this) == rhs);
}


void Atom::print(std::ostream & os) const
{
    print_output(os, "%-5?    %16.8?  %16.8?  %16.8?\n",
           atomic_symbol_from_z(Z), get_coords()[0], get_coords()[1], get_coords()[2]);
}

bphash::HashValue Atom::my_hash(void) const
{
    return bphash::make_hash(bphash::HashType::Hash128, *this);
} 

void Atom::hash(bphash::Hasher & h) const
{
    h(static_cast<const Point &>(*this),
           Z, isotope,
           mass, isotope_mass,
           charge, multiplicity,
           nelectrons, cov_radius,
           vdw_radius, basis_sets);
}

std::ostream& operator<<(std::ostream& os,const Atom& A)
{
    A.print(os);
    return os;
}


////////////////////////////////
// Free functions
////////////////////////////////

Atom create_atom(CoordType xyz, int Z)
{
    int isotope = most_common_isotope_from_z(Z);
    return create_atom(xyz, Z, isotope);

}

Atom create_atom(CoordType xyz, int Z, int isotope)
{
    return Atom(xyz,
                Z,
                isotope,
                atomic_mass_from_z(Z),
                isotope_mass_from_z(Z, isotope),
                0,  //! \todo default charge?
                numeric_cast<double>(atomic_multiplicity_from_z(Z)),
                numeric_cast<double>(Z), // nelectrons = Z
                covalent_radius_from_z(Z),
                vdw_radius_from_z(Z)
                );
}

Atom make_ghost_atom(const Atom & atom)
{
    Atom ghost = create_atom(atom.get_coords(), -1*abs(atom.Z));
    ghost.basis_sets = atom.basis_sets;
    return ghost;
}

bool is_ghost_atom(const Atom & atom)
{
    //! \todo simple enough check?
    return atom.Z < 0;
}


Atom make_dummy_atom(const CoordType & xyz)
{
    return create_atom(xyz, DUMMY_ATOM_Z);
}

Atom make_dummy_atom(const Atom & atom)
{
    return create_atom(atom.get_coords(), DUMMY_ATOM_Z);
}

bool is_dummy_atom(const Atom & atom)
{
    //! \todo simple enough check?
    return atom.Z == DUMMY_ATOM_Z;
}


Atom make_point_charge(const CoordType & xyz, double charge)
{
    Atom atom = create_atom(xyz, POINTCHARGE_ATOM_Z);
    atom.charge = charge;
    return atom;
}

Atom make_point_charge(const Atom & atom, double charge)
{
    return make_point_charge(atom.get_coords(), charge);
}

bool is_point_charge(const Atom & atom)
{
    //! \todo simple enough check?
    return atom.Z == POINTCHARGE_ATOM_Z;
}

} // close namespace pulsar

