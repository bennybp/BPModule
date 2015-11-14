#ifndef _GUARD_ATOM_HPP_
#define _GUARD_ATOM_HPP_

#include <array>

typedef std::array<double, 3> CoordType;

namespace bpmodule {
namespace molecule {


struct Atom
{
    unsigned long id;   //!< Unique id of this center
    int z;              //!< Atomic Z number
    int isonum;         //!< Isotope number
    CoordType xyz;      //!< XYZ coordinates

    //! Atomic mass for this atom/isotope
    double Mass(void) const;

    //! Name of the element
    std::string Name(void) const;

    //! Symbol of the element;
    std::string Symbol(void) const;
};


} // close namespace molecule
} // close namespace bpmodule

#endif

