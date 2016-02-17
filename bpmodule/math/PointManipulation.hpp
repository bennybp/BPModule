/*! \file
 *
 * \brief Manipulations of points and similar objects
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_MATH__POINTMANIPULATION_HPP_
#define BPMODULE_GUARD_MATH__POINTMANIPULATION_HPP_

#include "bpmodule/math/Cast.hpp"

namespace bpmodule {
namespace math {


/////////////////////////////////////////
// Translation
/////////////////////////////////////////

/*! \brief Translates a Point
 */
template<typename Point, typename Vector>
void TranslatePoint(Point & point, const Vector & vec)
{
    const auto & coords = point.GetCoords();

    point.SetCoords(vec[0] + coords[0],
                    vec[1] + coords[1],
                    vec[2] + coords[2]);
}



/*! \brief Translates a Point, returning a new Point
 */
template<typename Point, typename Vector3>
Point TranslatePoint_Copy(const Point & point, const Vector3 & vec)
{
    Point ret(point);
    TranslatePoint(ret, vec);
    return ret;
}



/*! \brief Translates a container of Points
 */
template<typename PointContainer, typename Vector3>
void TranslatePointContainer(PointContainer & cont, const Vector3 & vec)
{
    for(auto & it : cont)
        TranslatePoint<typename PointContainer::value_type, Vector3>(it, vec);
}



/*! \brief Translates a cotainer of Points, returning a new PointContainer
 */
template<typename PointContainer, typename Vector3>
PointContainer TranslatePointContainer_Copy(const PointContainer & cont, const Vector3 & vec)
{
    PointContainer ret(cont);
    TranslatePointContainer<PointContainer, Vector3>(ret, vec);
    return ret;    
}




////////////////////////////////////////////
// Rotation
////////////////////////////////////////////

/*! \brief Rotates a Point via a FlatMatrix
 */
template<typename Point, typename FlatMatrix9>
void RotatePoint(Point & point, const FlatMatrix9 & mat)
{
    point[0] = mat[0]*point[0] + mat[1]*point[1] + mat[2]*point[2];
    point[1] = mat[3]*point[0] + mat[4]*point[1] + mat[5]*point[2];
    point[2] = mat[6]*point[0] + mat[7]*point[1] + mat[8]*point[2];
}



/*! \brief Rotates a Point via a FlatMatrix, returning a new Point
 */
template<typename Point, typename FlatMatrix9>
Point RotatePoint_Copy(const Point & point, const FlatMatrix9 & mat)
{
    Point ret(point);
    RotatePoint(ret, mat);
    return ret;    
}



/*! \brief Rotates a container of Points via a FlatMatrix
 */
template<typename PointContainer, typename FlatMatrix9>
void RotatePointContainer(PointContainer & cont, const FlatMatrix9 & mat)
{
    for(auto & it : cont)
        RotatePoint(it, mat);
}



/*! \brief Rotates a container of Points via a FlatMatrix, returning a new PointContainer
 */
template<typename PointContainer, typename FlatMatrix9>
PointContainer RotatePointContainer_Copy(const PointContainer & cont, const FlatMatrix9 & vec)
{
    PointContainer ret(cont);
    RotatePointContainer(ret, vec);
    return ret;
}






////////////////////////////////////////////
// Center-finding
////////////////////////////////////////////

/*! \brief Find the center of a container of points
 */
template<typename Coord, typename PointContainer>
Coord PointsCenter(const PointContainer & cont)
{
    typedef typename Coord::value_type type_t;

    std::array<type_t, 3> coord{0,0,0};

    size_t n = 0;
    for(const auto & it : cont)
    {
        coord[0] += it[0];
        coord[1] += it[1];
        coord[2] += it[2];
        n++;
    }

    typename Coord::value_type n2 = math::numeric_cast<type_t>(n);

    coord[0] /= n2;
    coord[1] /= n2;
    coord[2] /= n2;

    return Coord{coord[0], coord[1], coord[2]};
}


/*! \brief Find the weighted center of a container of points
 */
template<typename Coord, typename PointContainer>
Coord WeightedPointsCenter(const PointContainer & cont, std::function<double (const typename PointContainer::value_type &)> weighter)
{
    typedef typename Coord::value_type type_t;

    std::array<type_t, 3> coord{0,0,0};

    type_t sumweight = 0;
    for(const auto & it : cont)
    {
        // the weighter function returns type R, and we need to cast it to type_t
        const type_t weight = math::numeric_cast<type_t>(weighter(it));

        coord[0] += weight * it[0];
        coord[1] += weight * it[1];
        coord[2] += weight * it[2];
        sumweight += weight;
    }

    coord[0] /= sumweight;
    coord[1] /= sumweight;
    coord[2] /= sumweight;
    return Coord{coord[0], coord[1], coord[2]};
}


} // close namespace math
} // close namespace bpmodule

#endif
