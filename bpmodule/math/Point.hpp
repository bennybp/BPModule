/*! \file
 *
 * \brief A simple class representing a Point
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef BPMODULE_GUARD_MATH__POINT_HPP_
#define BPMODULE_GUARD_MATH__POINT_HPP_

#include <array>
#include <cmath>

#include "bpmodule/pragma.h"
#include "bpmodule/util/Serialization.hpp"


namespace bpmodule{
namespace math{


/*! \brief A simple class representing a Point
 *
 * Cartesian point of data type \p T
 */
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
        virtual ~PointT()                  = default;

        bool operator==(const PointT & rhs) const
        {
            PRAGMA_WARNING_PUSH
            PRAGMA_WARNING_IGNORE_FP_EQUALITY
            return coords_ == rhs.coords_;
            PRAGMA_WARNING_POP
        }

        T & operator[](size_t i) { return coords_[i]; }
        T & at(size_t i) { return coords_.at(i); }

        const T & operator[](size_t i) const { return coords_[i]; }
        const T & at(size_t i) const { return coords_.at(i); }
        

        CoordType GetCoords(void) const { return coords_; }
        double GetCoord(int i) const { return coords_.at(i); }
        void SetCoord(int i, double val) const { coords_.at(i) = val; }

        void SetCoords(const CoordType & coords) { coords_ = coords; }
        void SetCoords(T x, T y, T z) { coords_ = CoordType{x,y,z}; }

        
        /** \brief Makes this the element-wise difference between this and 
         *         another point
         *
         *   \param[in] RHS The other point
         *   \return this with its new elements
         */
        PointT<T>& operator-=(const PointT<T>& RHS){
            for(size_t i=0;i<coords_.size();++i)
                this->coords_[i]-=RHS.coords_[i];
            return *this;
        }
        
        /** \brief Returns the element-wise difference between this and 
         *         another point
         *
         *   \param[in] RHS The other point
         *   \return A copy located at a point that is the element-wise 
         *           difference of this and RHS
         */
        PointT<T> operator-(const PointT<T>& RHS)const{
            return PointT<T>(*this)-=RHS;
        }
        
        /** \brief Makes this the element-wise sum between this and 
         *         another point
         *
         *   \param[in] RHS The other point
         *   \return this with its new elements
         */
        PointT<T>& operator+=(const PointT<T>& RHS){
            for(size_t i=0;i<coords_.size();++i)
                this->coords_[i]+=RHS.coords_[i];
            return *this;
        }
        
        /** \brief Returns the element-wise difference between this and 
         *         another point
         *
         *   \param[in] RHS The other point
         *   \return A copy located at a point that is the element-wise 
         *           difference of this and RHS
         */
        PointT<T> operator+(const PointT<T>& RHS)const{
            return PointT<T>(*this)+=RHS;
        }
        
        /** \brief Makes this the element-wise product between this and 
         *         another point
         *
         *   \param[in] RHS The other point
         *   \return this with its new elements
         */
        PointT<T>& operator*=(const PointT<T>& RHS){
            for(size_t i=0;i<coords_.size();++i)
                this->coords_[i]*=RHS.coords_[i];
            return *this;
        }
        
        
        /** \brief Returns the element-wise product between this and 
         *         another point
         *
         *   \param[in] RHS The other point
         *   \return The element-wise product of this and RHS
         */
        PointT<T> operator*(const PointT<T>& RHS)const{
            return PointT<T>(*this)*=RHS;
        }
        
        ///Returns the magnitude of this point (sqrt of dot product)
        double Magnitude()const{
            PointT<T> Sq=(*this)*(*this);
            double sum=0.0;
            for(size_t i=0;i<coords_.size();++i)sum+=Sq[i];
            return sqrt(sum);
        }

        ///Returns the distance between this point and RHS
        double Distance(const PointT<T>& RHS)const{
            PointT<T> Diff=*this-RHS;
            return Diff.Magnitude();
        }


    private:
        CoordType coords_;


        //! \name Serialization
        ///@{
        
        DECLARE_SERIALIZATION_FRIENDS
        
        template<class Archive>
        void serialize(Archive & ar)
        {
            ar(coords_);
        }

        ///@}
};


typedef PointT<double> Point;


}
}//End namespaces

#endif /* BPMODULE_GUARD_POINT_HPP_ */


