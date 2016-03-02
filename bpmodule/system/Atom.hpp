#ifndef BPMODULE_GUARD_SYSTEM__ATOM_HPP_
#define BPMODULE_GUARD_SYSTEM__ATOM_HPP_

#include "bpmodule/math/Point.hpp"
#include "bpmodule/math/Cast.hpp"
#include "bpmodule/basisset/BasisShell.hpp"


namespace bpmodule {
namespace system {



/*! \brief A center in a molecule
 * 
 * Follows PointConcept via derivation from math::Point
 */
class Atom : public math::Point
{
    private:
        size_t idx_;   //!< Unique index of the atom
        int Z_;        //!< Atomic Z number (as integer. Also stored as a (double) weight)
        int isonum_;   //!< Isotope number

        double mass_;
        double isotopemass_;
        double charge_;
        double multiplicity_;
        double nelectrons_;

        basisset::ShellMap bshells_; //!< Basis functions associated with this atom/center

    public:
        typedef math::Point::CoordType CoordType;

        // constructor
        Atom(size_t idx,  CoordType xyz, int Z, int isonum,
             double charge, double multiplicity, double nelectrons)
        {
            // we do it this way in case we change where the info is stored
            idx_ = idx;
            SetCoords(xyz);
            SetZ(Z);
            SetIsonum(isonum);
            SetCharge(charge);
            SetMultiplicity(multiplicity);
            SetNElectrons(nelectrons);
        }



        /*
         * I'm aware that a class with lots of getters/setters is bad
         * form. However, we are leaving it open to future optimizations,
         * such as storing the data elsewhere
         */

        size_t GetIdx(void) const noexcept { return idx_; }

        int GetZ(void) const noexcept { return Z_; }
        void SetZ(int Z) noexcept { Z_ = Z; }

        int GetIsonum(void) const noexcept { return isonum_; }
        void SetIsonum(int isonum) noexcept { isonum_ = isonum; }

        double GetMass(void) const { return mass_; }
        void SetMass(double mass) { mass_ = mass; };

        double GetIsotopeMass(void) const { return isotopemass_; }
        void SetIsotopeMass(double isotopemass) { isotopemass_ = isotopemass; };

        double GetCharge(void) const noexcept { return charge_; }
        void SetCharge(double charge) noexcept { charge_ = charge; }

        double GetMultiplicity(void) const noexcept { return charge_; }
        void SetMultiplicity(double m) noexcept { multiplicity_ = m; }

        double GetNElectrons(void) const noexcept { return nelectrons_; }
        void SetNElectrons(double n) noexcept { nelectrons_ = n; }

        basisset::ShellMap GetAllShells(void) const { return bshells_; }
        basisset::ShellVector GetShells(const std::string & label) const { return bshells_.at(label); } //! \todo exceptions
        void SetShells(const std::string & label, const basisset::ShellVector & shells) { bshells_[label] = shells; }
        void AddShell(const std::string & label, const basisset::BasisShell & shell) { bshells_[label].push_back(shell); }


        bool operator==(const Atom & rhs) const;
        bool operator!=(const Atom & rhs) const;


        //! Name of the element
        std::string GetName(void) const;

        //! Symbol of the element;
        std::string GetSymbol(void) const;
};

std::ostream& operator<<(std::ostream& os,const Atom& A);


/* \brief Create an atom given an ID, coordinates, and atomic number
 *
 * The rest of the data is filled in automatically
 */
Atom CreateAtom(size_t idx, Atom::CoordType xyz, int Z);


/*! \copydocs CreateAtom(size_t,size_t, Atom::CoordType, int) */
Atom CreateAtom(size_t idx, double x, double y, double z, int Z);



/* \brief Create an atom given an ID, coordinates, atomic number, and isotope number
 *
 * The rest of the data is filled in automatically
 */
Atom CreateAtom(size_t idx, Atom::CoordType xyz, int Z, int isonum);


/*! \copydocs CreateAtom(size_t,size_t, Atom::CoordType, int, int) */
Atom CreateAtom(size_t idx, double x, double y, double z, int Z, int isonum);




} // close namespace system
} // close namespace bpmodule

#endif

