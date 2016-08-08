#ifndef PULSAR_GUARD_MATH__GRID_HPP_
#define PULSAR_GUARD_MATH__GRID_HPP_

#include "pulsar/math/MathSet.hpp"
#include "pulsar/math/Point.hpp"

namespace pulsar {
namespace math {

template<typename T>
struct GridPointT
{
    PointT<T> coords; // The coordinates of the point
    T value;          // The value for the point on the grid


    /// Return a unique has of the point
    bphash::HashValue my_hash(void) const
    {
        return bphash::make_hash(bphash::HashType::Hash128, *this);
    }

    bool operator==(const GridPointT & rhs) const
    {
        PRAGMA_WARNING_PUSH
        PRAGMA_WARNING_IGNORE_FP_EQUALITY

        return coords == rhs.coords &&
               value == rhs.value;

        PRAGMA_WARNING_POP
    }

    bool operator!=(const GridPointT & rhs) const
    {
        return ((*this) != rhs);
    }

    void print(std::ostream& os) const
    {
        os << coords << " " << value;
    }

    private:
        //! \name Serialization and Hashing
        ///@{
        
        DECLARE_SERIALIZATION_FRIENDS
        DECLARE_HASHING_FRIENDS
        
        template<class Archive>
        void serialize(Archive & ar)
        {
            ar(coords, value);
        }

        void hash(bphash::Hasher & h) const
        {
            h(coords, value);
        }

        ///@}
};




// A grid is just a MathSet of grid points
template<typename T>
using GridT = MathSet<GridPointT<T>>;

template<typename T>
using GridUniverseT = Universe<GridPointT<T>>; 


// Convenience
typedef GridPointT<double> GridPoint;
typedef GridUniverseT<double> GridUniverse;
typedef GridT<double> Grid;


template<typename T>
std::ostream& operator<<(std::ostream& os, const GridPointT<T>& P)
{
    P.print(os);
    return os;
}


} // close namespace math
} // close namespace pulsar

#endif
