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

using namespace pulsar::output;

namespace pulsar{

namespace system {


/////////////////////////////////////////
// BasisInfo
/////////////////////////////////////////

PRAGMA_WARNING_PUSH
PRAGMA_WARNING_IGNORE_SHADOW_MEMBER

Atom::Atom(CoordType xyz, int Z, int isotope, double mass,
           double isotope_mass, double charge, double multiplicity,
           double nelectrons,double cov_radius, double vdw_radius)
    : math::Point(xyz), Z(Z), isotope(isotope), mass(mass),
      isotope_mass(isotope_mass), charge(charge), multiplicity(multiplicity),
      nelectrons(nelectrons), cov_radius(cov_radius), vdw_radius(vdw_radius)
{ }

PRAGMA_WARNING_POP


bool Atom::operator==(const Atom & rhs) const
{
    PRAGMA_WARNING_PUSH
    PRAGMA_WARNING_IGNORE_FP_EQUALITY

    // order by the parts that are most likely to be different, since
    //   this should short-circuit on the first false comparison
    return Z == rhs.Z &&
           static_cast<const math::Point>(*this) == static_cast<const math::Point>(rhs) &&
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
    h(static_cast<const math::Point &>(*this),
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
                math::numeric_cast<double>(atomic_multiplicity_from_z(Z)),
                math::numeric_cast<double>(Z), // nelectrons = Z
                covalent_radius_from_z(Z),
                vdw_radius_from_z(Z)
                );
}

Atom create_atom(double x, double y, double z, int Z)
{
    return create_atom({x,y,z}, Z);
}

Atom create_atom(double x, double y, double z, int Z, int isotope)
{
    return create_atom({x,y,z}, Z, isotope);
}


Atom make_ghost_atom(const Atom & atom)
{
    Atom ghost = create_atom(atom.get_coords(), GHOST_ATOM_Z);
    ghost.basis_sets = atom.basis_sets;
    return ghost;
}

bool is_ghost_atom(const Atom & atom)
{
    //! \todo simple enough check?
    return atom.Z == GHOST_ATOM_Z;
}


Atom make_dummy_atom(const CoordType & xyz)
{
    return create_atom(xyz, DUMMY_ATOM_Z);
}

Atom make_dummy_atom(const Atom & atom)
{
    return create_atom(atom.get_coords(), DUMMY_ATOM_Z);
}

Atom make_dummy_atom(double x, double y, double z)
{
    return create_atom({x, y, z}, DUMMY_ATOM_Z);
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

Atom make_point_charge(const Atom & atom)
{
    return make_point_charge(atom.get_coords(), atom.charge);
}

Atom make_point_charge(double x, double y, double z, double charge)
{
    return make_point_charge({x,y,z}, charge);
}

bool is_point_charge(const Atom & atom)
{
    //! \todo simple enough check?
    return atom.Z == POINTCHARGE_ATOM_Z;
}


} // close namespace system
} // close namespace pulsar

