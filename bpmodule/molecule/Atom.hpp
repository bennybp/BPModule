#ifndef _GUARD_ATOM_HPP_
#define _GUARD_ATOM_HPP_

#include <array>

typedef std::array<double, 3> CoordType;

namespace bpmodule {
namespace molecule {


class Atom
{
    public:
        Atom(unsigned long id, int z, int isonum, double mass, CoordType xyz)
            : id_(id), z_(z), isonum_(isonum), mass_(mass), xyz_(xyz)
        { }

        // Compiler-generated are ok
        Atom(const Atom & rhs)             = default;
        Atom(Atom && rhs)                  = default;
        Atom & operator=(const Atom & rhs) = default;
        Atom & operator=(Atom && rhs)      = default;

        ~Atom() = default;

        /*! \brief Retrieve the unique ID number */
        unsigned long ID(void) const noexcept { return id_; }

  
        /*! \brief Retrieve the atomic Z number */
        int Z(void) const noexcept { return z_; }


        /*! \brief Retrieve the isotope number */
        int Isonum(void) const noexcept { return isonum_; }

        /*! \brief Retrieve the atomic mass */
        double Mass(void) const noexcept { return mass_; }

        /*! \brief Retrieve coordinates */
        const CoordType & XYZ(void) const noexcept { return xyz_; }

        /*! \brief Get a specific coordinate (0 = x, 1 = y, 2 = z) */
        double GetCoord(int idx) const
        {
            //! \todo check that 0 <= idx <= 2
            return xyz_[idx];
        }

        /*! \brief Get a specific coordinate (0 = x, 1 = y, 2 = z) */
        double operator[](int idx) const { return GetCoord(idx); }

    private:
        unsigned long id_; //!< Unique id of this center
        int z_;            //!< Atomic Z number
        int isonum_;       //!< Isotope number
        double mass_;      //!< Atomic mass for this atom/isotope

        CoordType xyz_;
};


} // close namespace molecule
} // close namespace bpmodule

#endif

