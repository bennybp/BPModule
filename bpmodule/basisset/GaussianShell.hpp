#ifndef _GUARD_GAUSSIANSHELL_HPP_
#define _GUARD_GAUSSIANSHELL_HPP_

#include <array>
#include "bpmodule/basisset/NCartesian.hpp"

namespace bpmodule {
namespace basisset {

typedef std::array<double, 3> CoordType;


// for friend class
class BasisSet;


//! \todo maybe GaussianShell privately derives from GaussianBasisShell?
//        this would remove the AddPrimitive function, which might be desirable

class GaussianBasisShell
{
    public:
        GaussianBasisShell(int am, bool cart)
            : am_(am), cart_(cart)
        { }


        // compiler generated ok
        GaussianBasisShell(const GaussianBasisShell & rhs)             = default;
        GaussianBasisShell(GaussianBasisShell && rhs)                  = default;
        GaussianBasisShell & operator=(const GaussianBasisShell & rhs) = default;
        GaussianBasisShell & operator=(GaussianBasisShell && rhs)      = default;

        void AddPrimitive(double alpha, double coef)
        {
            alphas_.push_back(alpha);
            coefs_.push_back(coef);
        }


        int AM(void) const noexcept { return am_; }

        int NPrim(void) const noexcept { return static_cast<int>(alphas_.size()); }
        int NCartesian(void) const noexcept { return NCARTESIAN(am_); }
        int NSpherical(void) const noexcept { return NSPHERICAL(am_); }
        int NFunctions(void) const noexcept { return (cart_ ? NCartesian() : NSpherical()); } 

        bool IsCartesian(void) const noexcept { return cart_; }
        bool IsSpherical(void) const noexcept { return !cart_; }


        //! \todo check if zero nprim?
        std::vector<double> Alphas(void) const noexcept { return alphas_; }
        std::vector<double> Coefs(void) const noexcept { return coefs_; }


        //! \todo wrap exceptions
        double Alpha(int i) const { return alphas_.at(i); }
        double Coef(int i) const { return coefs_.at(i); }


    private:
        int am_;                    //!< Angular momentum
        bool cart_;                 //!< Is cartesian?
        std::vector<double> alphas_; //!< Exponents
        std::vector<double> coefs_; //!< Exponents
};


class GaussianShell : public GaussianBasisShell
{
    public:
        //! \todo construct from python list
        GaussianShell(const GaussianBasisShell & gbs, unsigned long id,
                      unsigned long center, double x, double y, double z)
            : GaussianBasisShell(gbs), id_(id), center_(center), xyz_{x,y,z}
        { }

        GaussianShell(int am, bool cart, unsigned long id,
                      unsigned long center, double x, double y, double z)
            : GaussianBasisShell(am, cart), id_(id), center_(center), xyz_{x,y,z}
        { }

        GaussianShell(const GaussianBasisShell & gbs, unsigned long id,
                      unsigned long center, const CoordType & xyz)
            : GaussianBasisShell(gbs), id_(id), center_(center), xyz_(xyz)
        { }

        GaussianShell(int am, bool cart, unsigned long id,
                      unsigned long center, const CoordType & xyz)
            : GaussianBasisShell(am, cart), id_(id), center_(center), xyz_(xyz)
        { }

        // compiler generated ok
        GaussianShell(const GaussianShell & rhs)             = default;
        GaussianShell(GaussianShell && rhs)                  = default;
        GaussianShell & operator=(const GaussianShell & rhs) = default;
        GaussianShell & operator=(GaussianShell && rhs)      = default;
        
        unsigned long ID(void) const noexcept { return id_; }
        unsigned long Center(void) const noexcept { return center_; }

        CoordType Coordinates(void) const { return xyz_; }


    protected:
        friend class BasisSet;

        void SetID(unsigned long id) { id_ = id; }


    private:
        unsigned long id_;          //!< Unique id for this shell
        unsigned long center_;      //!< ID of the center
        CoordType xyz_;             //!< XYZ coordindates of this center
    
};


} // close namespace basisset
} // close namespace bpmodule


#endif
