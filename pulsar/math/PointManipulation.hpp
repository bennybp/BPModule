/*! \file
 *
 * \brief Manipulations of points and similar objects
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_MATH__POINTMANIPULATION_HPP_
#define PULSAR_GUARD_MATH__POINTMANIPULATION_HPP_

#include "pulsar/math/Cast.hpp"
#include "pulsar/util/IterTools.hpp"

namespace pulsar{
namespace math {


/////////////////////////////////////////
// Translation
/////////////////////////////////////////

/*! \brief Translates a Point
 */
template<typename Point, typename Vector>
void TranslatePoint(Point & point, const Vector & vec)
{
    point[0] += vec[0];
    point[1] += vec[1];
    point[2] += vec[2];
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
    auto tmp0 = point[0];
    auto tmp1 = point[1];
    auto tmp2 = point[2];
    point[0] = mat[0]*tmp0 + mat[1]*tmp1 + mat[2]*tmp2;
    point[1] = mat[3]*tmp0 + mat[4]*tmp1 + mat[5]*tmp2;
    point[2] = mat[6]*tmp0 + mat[7]*tmp1 + mat[8]*tmp2;
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

///Converts a container of points into a std::vector<double>
template<typename PointContainer>
std::vector<double> ToDoubleStar(const PointContainer& pc){
    std::vector<double> RV;
    for(const auto& point: pc)
        for(size_t i: util::Range<0,3>())
            RV.push_back(point[i]);
    return RV;
}




////////////////////////////////////////////
// Center-finding
////////////////////////////////////////////

///Recursive function for computing the moments (Don't directly call)
template<size_t M,typename Pt>
void MomentGuts(std::vector<size_t> Index,const Pt& P,const size_t& Dim,
                double value,std::vector<double>& Moments){
    if(Index.size()==M-1){
        size_t Offset=0,stride=Dim;
        for(size_t i=Index.size();i>0;--i){
            Offset+=Index[i-1]*stride;
            stride*=Dim;
        }
        for(size_t i=0;i<Dim;++i)
            Moments[Offset+i]+=value*P[i];
    }
    else{
        Index.push_back(0);
        for(size_t i=0;i<Dim;++i){
            Index.back()=i;
            MomentGuts<M>(Index,P,Dim,value*P[i],Moments);
        }
    }
    
}

/*! \brief Finds the normalized M-th moment of a distribution of points
 *
 * 
 *  For a series of \f$n\f$ points, centered about the origin, such that the
 *  \f$i\f$-th point, \f$X_i\f$, is weighted by \f$w_i\f$, the \f$M\f$-th 
 *  moment, \f$R^{(M)}\f$ is given by:
 *  \f[
 *    R^{(M)}=\frac{\sum_{i=1}^M w_i\bigotimes_{j=1}^MX_i}{\sum_{i=1}^M w_i}
 *  \f]
 * 
 * Function assumes for the time-being that all points have 3 components, but
 * that can be changed by setting the Dim parameter appropriately.
 * 
 * \param[in] The container of points, must:
 *    - Define the type value_type as the type that it holds
 *      - value_type must by index accessible (define operator[])
 *    - Have iterators
 * \param[in] fxn A function that given an object of value_type type returns
 *                its weight as a double.  A default "weighter" is provided 
 *                that weights each point equally.
 * \return The flattened moments.  Each component runs from [0,Dim)
 * 
 * \todo if dim is always going to be known at compile time switch from vector
 *       to array
 * 
 */
template<size_t M,typename PointContainer>
std::vector<double> Moment(const PointContainer& cont,
  std::function<double (const typename PointContainer::value_type &)> fxn
  =[](const typename PointContainer::value_type &){return 1.0;}){
    if(M==0)return {1.0};//Handle 0-th normalized moment
    const size_t Dim=3;
    size_t DoF=1;
    for(size_t i=0;i<M;++i)DoF*=Dim;
    std::vector<double> Moments(DoF);
    double Total=0.0;
    for(const auto& Xi: cont){
        double value=fxn(Xi);
        Total+=value;
        MomentGuts<M>(std::vector<size_t>(),Xi,Dim,value,Moments);
    }
    for(size_t i=0;i<DoF;++i)Moments[i]/=Total;
    return Moments;
}

/*! \brief Find the weighted center of a container of points
 */
template<typename Coord, typename PointContainer>
Coord WeightedPointsCenter(const PointContainer & cont, std::function<double (const typename PointContainer::value_type &)> weighter)
{
     std::vector<double> coord=Moment<1>(cont,weighter);
     return Coord{coord[0],coord[1],coord[2]};
}

template<typename Coord, typename PointContainer>
Coord PointsCenter(const PointContainer & cont)
{
    return WeightedPointsCenter<Coord>(cont,
           [](const typename PointContainer::value_type&){return 1.0;});
}

} // close namespace math
} // close namespace pulsar

#endif
