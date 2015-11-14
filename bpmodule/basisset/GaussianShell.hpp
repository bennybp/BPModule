#ifndef _GUARD_GAUSSIANSHELL_HPP_
#define _GUARD_GAUSSIANSHELL_HPP_

#include <array>

namespace bpmodule {
namespace basisset {

typedef std::array<double, 3> CoordType;


// for friend class
class BasisSet;


class GaussianShell
{
    public:
        GaussianShell(int am, bool cart, CoordType xyz)
            : id_(0), am_(am), cart_(cart), xyz_(xyz)
        { }


        // compiler generated ok
        GaussianShell(const GaussianShell & rhs)             = default;
        GaussianShell(GaussianShell && rhs)                  = default;
        GaussianShell & operator=(const GaussianShell & rhs) = default;
        GaussianShell & operator=(GaussianShell && rhs)      = default;

        void AddPrimitive(double alpha, double coef)
        {
            alphas_.push_back(alpha);
            coefs_.push_back(coef);
        }


        unsigned long ID(void) const noexcept { return id_; }
        int AM(void) const noexcept { return am_; }

        int NPrim(void) const noexcept { return static_cast<int>(alphas_.size()); }
        int NCartesian(void) const noexcept { return (((am_+1)*(am_+2)) >> 1); }
        int NSpherical(void) const noexcept { return (2*am_+1); }
        int NFunctions(void) const noexcept { return (cart_ ? NCartesian() : NSpherical()); } 

        bool IsCart(void) const noexcept { return cart_; }
        bool IsSpherical(void) const noexcept { return !cart_; }


        //! \todo check if zero nprim?
        double const * Alphas(void) const noexcept { return alphas_.data(); }
        double const * Coefs(void) const noexcept { return coefs_.data(); }


        //! \todo wrap exceptions
        double Alpha(size_t i) const { return alphas_.at(i); }
        double Coef(size_t i) const { return coefs_.at(i); }

        CoordType Coordinates(void) const { return xyz_; }

    protected:
        friend class BasisSet;

        void SetID(unsigned long id) { id_ = id; }

    private:
        unsigned long id_;          //!< Unique id for this shell
        int am_;                    //!< Angular momentum
        bool cart_;                 //!< Is cartesian?
        CoordType xyz_;             //!< XYZ coordindates of this center
        std::vector<double> alphas_; //!< Exponents
        std::vector<double> coefs_; //!< Exponents
    
};


} // close namespace basisset
} // close namespace bpmodule


#endif
