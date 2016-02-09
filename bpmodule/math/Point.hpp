#ifndef BPMODULE_GUARD_POINT_HPP_
#define BPMODULE_GUARD_POINT_HPP_

#include <array>
#include <algorithm>

namespace bpmodule{
namespace math{


// Templated, because why not
template<typename T, typename WeightType>
class WeightedPointT
{
    public:
        typedef std::array<T, 3> CoordType;
        typedef std::array<T, static_cast<size_t>(WeightType::NWEIGHTS)> WeightArray;
   

        /*! \brief Construct via coordinates
         *
         * All weights set to 0
         */
        WeightedPointT(const CoordType & coords)
            : coords_(coords)
        {   
            std::fill(weights_.begin(), weights_.end(), static_cast<T>(0));
        }
 
 
        /*! \brief Construct via coordinates
         *
         * All weights set to 0
         */
        WeightedPointT(T x, T y, T z)
            : WeightedPointT(CoordType{x, y, z})
        { }
 
 
        /*! \brief Construct via coordinates and weights
         */
        WeightedPointT(const CoordType & coords, const WeightArray & weights)
            : coords_(coords), weights_(weights)
        { }


        /*! \brief Construct via coordinates and weights
         */
        WeightedPointT(T x, T y, T z, const WeightArray & weights)
            : WeightedPointT({x,y,z}, weights_(weights))
        { }

 
        /*! \brief Default constructor
         * Everything is set to zero
         */
        WeightedPointT() : WeightedPointT(0, 0, 0) { }
 

        /*! \brief Construct via another WeightedPointT, but with new coordinates
         */ 
        WeightedPointT(const WeightedPointT & rhs, T x, T y, T z)
            : WeightedPointT(x, y, z, rhs.weights_)
        { } 

 
        /*! \brief Construct via another WeightedPointT, but with new coordinates
         */ 
        WeightedPointT(const WeightedPointT & rhs, const CoordType & xyz)
            : WeightedPointT(xyz, rhs.weights_)           
        { }

 
        // everything else compiler-generated should be ok
        WeightedPointT(const WeightedPointT &) = default;
        WeightedPointT(WeightedPointT &&) = default;
        WeightedPointT & operator=(const WeightedPointT &) = default;
        WeightedPointT & operator=(WeightedPointT &&) = default;
        

        /*! \brief Get the coordinates of this point
         */
        CoordType & Coords(void) noexcept { return coords_; }


        /*! \brief Get the coordinates of this point
         */
        const CoordType & Coords(void) const noexcept { return coords_; }


        /*! \brief Get a single component of the coordinates
         */
        T & Coord(int i) { return coords_.at(i); }


        /*! \brief Get a single component of the coordinates
         */
        const T & Coord(int i) const { return coords_.at(i); }


        /*! \brief Get a weight associated with this point
         */
        T & Weight(WeightType weight) { return weights_[static_cast<size_t>(weight)]; }


        /*! \brief Get a weight associated with this point
         */
        const T & Weight(WeightType weight) const { return weights_[static_cast<size_t>(weight)]; }
        

    private:
        CoordType coords_;
        CoordType weights_;
};       


enum class EmptyPointWeights
{
    NWEIGHTS = 0
};


// typedefs
template<typename WeightType>
using WeightedPoint = WeightedPointT<double, WeightType>;


// Normal, unweighted points
typedef WeightedPoint<EmptyPointWeights> Point;


}
}//End namespaces

#endif /* BPMODULE_GUARD_POINT_HPP_ */


