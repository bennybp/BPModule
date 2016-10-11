/*! \file
 *
 * \brief A simple class representing a Point
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_MATH__POINT_HPP_
#define PULSAR_GUARD_MATH__POINT_HPP_

#include <array>
#include <cmath>

#include "pulsar/pragma.h"
#include "pulsar/util/Serialization.hpp"
#include "bphash/Hasher.hpp"
#include "bphash/types/array.hpp"


namespace pulsar{
namespace math{


/*! \brief A simple class representing a Point
 *
 * Cartesian point of data type \p T
 *
 * \par Hashing
 * The hash value of a point represents a unique value
 * for the coordinates. 
 */
template<typename T>
class PointT
{
    public:
        ///The type storing the coordinates
        typedef std::array<T, 3> CoordType;
        ///The type of each coordinate
        typedef typename CoordType::value_type value_type;
        ///A constant iterator over the coordinates
        typedef typename CoordType::const_iterator const_iterator;
        ///An iterator over the coordinates
        typedef typename CoordType::iterator iterator;
        
        ///Makes a point given an std::array
        PointT(const CoordType & coords)
            : coords_(coords)
        { }

        ///Given individual coordinates fills point
        PointT(T x, T y, T z)
            : coords_{x,y,z}
        { }            

        ///Default constructor, defaults each coordinate
        PointT()                           = default;
        ///Deep copies another point
        PointT(const PointT &)             = default;
        ///Takes ownership of another point
        PointT(PointT &&)                  = default;
        ///Deep copy assignment
        PointT & operator=(const PointT &) = default;
        ///Assignment by taking ownership
        PointT & operator=(PointT &&)      = default;
        ///Destructor does nothing
        virtual ~PointT()                  = default;
        
        ///Returns the number of coordinates in the point
        constexpr size_t size()const{return coords_.size();}
        
        ///Returns a constant iterator to the beginning of the coordinate
        const_iterator begin()const{return coords_.begin();}
        ///Returns an iterator to the beginning of the coordinate
        iterator begin(){return coords_.begin();}
        const_iterator end()const{return coords_.end();}
        iterator end(){return coords_.end();}
        
        bool operator==(const PointT & rhs) const
        {
            PRAGMA_WARNING_PUSH
            PRAGMA_WARNING_IGNORE_FP_EQUALITY
            return coords_ == rhs.coords_;
            PRAGMA_WARNING_POP
        }

        bool operator!=(const PointT & rhs) const
        {
            return !((*this) == rhs);
        }

        T & operator[](size_t i) { return coords_[i]; }
        T & at(size_t i) { return coords_.at(i); }

        const T & operator[](size_t i) const { return coords_[i]; }
        const T & at(size_t i) const { return coords_.at(i); }
        

        CoordType get_coords(void) const { return coords_; }
        double get_coord(int i) const { return coords_.at(i); }
        void set_coord(int i, double val) const { coords_.at(i) = val; }

        void set_coords(const CoordType & coords) { coords_ = coords; }
        void set_coords(T x, T y, T z) { coords_ = CoordType{x,y,z}; }

        
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
        
        /** \brief Scales the point by a scalar*/
        PointT<T>& operator*=(double c){
            for(T& i: coords_)i*=c;
            return *this;
        }
        
        /** \brief Scales a copy of the point by a scalar*/
        PointT<T> operator*(double c)const{
            return PointT<T>(*this)*=c;
        }
        
        ///Returns the magnitude of this point (sqrt of dot product)
        double magnitude()const{
            PointT<T> Sq=(*this)*(*this);
            double sum=0.0;
            for(size_t i=0;i<coords_.size();++i)sum+=Sq[i];
            return sqrt(sum);
        }

        ///Returns the distance between this point and RHS
        double distance(const PointT<T>& RHS)const{
            PointT<T> Diff=*this-RHS;
            return Diff.magnitude();
        }
        
        void print(std::ostream& os)const{
            for(size_t i=0;i<coords_.size();++i){
                os<<coords_[i];
                if(i!=coords_.size()-1)os<<" ";
            }
        }

        /// Return a unique has of the point
        bphash::HashValue my_hash(void) const
        {
            return bphash::make_hash(bphash::HashType::Hash128, *this);
        }

    private:
        CoordType coords_;


        //! \name Serialization and Hashing
        ///@{
        
        DECLARE_SERIALIZATION_FRIENDS
        BPHASH_DECLARE_HASHING_FRIENDS
        
        template<class Archive>
        void serialize(Archive & ar)
        {
            ar(coords_);
        }

        void hash(bphash::Hasher & h) const
        {
            h(coords_);
        }

        ///@}
};

///Allows for left-multiplication by a double
template<typename T>
PointT<T> operator*(double c,const PointT<T>& P){
    return PointT<T>(P)*=c;
}

template<typename T>
std::ostream& operator<<(std::ostream& os,const PointT<T>& P){
    P.print(os);
    return os;
}

typedef PointT<double> Point;




}}//End namespaces

#endif /* PULSAR_GUARD_POINT_HPP_ */


