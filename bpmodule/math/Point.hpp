#ifndef BPMODULE_GUARD_POINT_HPP_
#define BPMODULE_GUARD_POINT_HPP_

#include <array>

namespace bpmodule{
namespace math{



template<typename T>
class PointT
{
    public:
        typedef std::array<T, 3> CoordType;
        typedef typename CoordType::value_type value_type;

        PointT(const CoordType & coords)
            : coords_(coords)
        { }

        PointT(T x, T y, T z)
            : coords_{x,y,z}
        { }


        PointT()                           = default;
        PointT(const PointT &)             = default;
        PointT(PointT &&)                  = default;
        PointT & operator=(const PointT &) = default;
        PointT & operator=(PointT &&)      = default;

        T & operator[](size_t i) { return coords_[i]; }
        T & at(size_t i) { return coords_.at(i); }

        const T & operator[](size_t i) const { return coords_[i]; }
        const T & at(size_t i) const { return coords_.at(i); }
        

        CoordType GetCoords(void) const { return coords_; }

        void SetCoords(const CoordType & coords) { coords_ = coords; }

        void SetCoords(T x, T y, T z) { coords_ = CoordType{x,y,z}; }

    private:
        CoordType coords_;

};


typedef PointT<double> Point;


}
}//End namespaces

#endif /* BPMODULE_GUARD_POINT_HPP_ */


