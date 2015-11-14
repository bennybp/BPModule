#ifndef _GUARD_ATOM_HPP_
#define _GUARD_ATOM_HPP_

#include <array>

typedef std::array<double, 3> CoordType;

namespace bpmodule {
namespace molecule {


struct Atom
{
    unsigned long id; //!< Unique id of this center
    int z;            //!< Atomic Z number
    int isonum;       //!< Isotope number
    double mass;      //!< Atomic mass for this atom/isotope

    CoordType xyz;
};


} // close namespace molecule
} // close namespace bpmodule

#endif

