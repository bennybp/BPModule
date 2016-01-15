/** 
 * @file Locus.hpp
 * Original author:
 * @Author Ryan Richard (ryanmrichard1@gmail.com)
 * With other contributions from:
 *
 * Creation date:
 * @date Jan 15, 2016
 * @brief TODO: Describe this file
 *
 *
 * TODO: Write a longer description of this file
 */
#ifndef BPMODULE_MATH_LOCUS_HPP_
#define BPMODULE_MATH_LOCUS_HPP_
#include <vector>
#include <map>
#include <string>
#include "bpmodule/math/WeightType.hpp"

namespace bpmodule{
namespace math{

class Locus;

/** \brief Wrapper class for a Cartesian point
 *
 *  To the user. This is nothing more than a point in Cartesian space.  If
 *  we choose to do so we may also associate some weights with this point.
 *  This class is not separable from the Locus class.
 *
 *  To the developer.  We get fancy here.  Basically the user is going to
 *  use this class to shuffle information around in a very
 *  logical manner.  We however, have to worry about efficiency.
 *  Specifically, if these points are say grid points we want the coordinates
 *  to be contiguous as well as the weights. (May want to dot product them).
 *  To do this, we store the actual weights and points in an associated
 *  Locus class.  This class then only contains pointers to the weights
 *  and carts.  These pointers are invalidated any time the Locus class
 *  associated with this point is changed (point added/deleted, Locus goes
 *  out of scope, etc.).  For all intents and purposes, this class has
 *  the same limitations as an std::vector's iterators.  Basically, don't
 *  store Point instances, make them when you want them and then let them
 *  go.
 *
 */
class Point{
   private:
      ///Allows Locus to set pointers
      friend Locus;

      ///The Locus associated with this Point
      Locus* Locus_=nullptr;

      ///The weights associated with this point
      std::map<WeightType,double*> Weights_;

      ///Did we allocate the Weights memory
      bool WeOwnWeights_=false;

      ///A pointer to the coordinates of this point
      double* Carts_=nullptr;

      ///Did we allocate the Carts memory
      bool WeOwnCarts_=false;

   public:
      ///Creates a new NULL Point
      Point(){}

      ///Creates a point with coordinates x,y,z
      Point(double x,double y, double z);

      ///Takes any iterators to a set of three components each of type double
      template<typename Itr_t>
      Point(Itr_t& Itr,const Itr_t& ItrEnd):Carts_(new double[3]),
      WeOwnCarts_(true){
         for(size_t I=0;Itr!=ItrEnd;++Itr,++I)Carts_[I]=*Itr;
      }

      ///Deletes memory (if we own it)
      ~Point();

      ///Adds a weight of type WT (overwrites if exists)
      void AddWeight(const WeightType& WT,double w);

      ///Returns the number of weights this point has
      size_t NWeights()const noexcept{return Weights_.size();}

      ///Returns the value of the weight with WeightType WT
      double Weight(const WeightType& WT)const;

      ///Returns the I-th cart of this point
      double operator[](size_t I)const;

      ///Converts a point to a string for printing
      explicit operator std::string()const;


};

///Just prints the carts
inline std::ostream& operator<<(std::ostream& os,const Point& p){
      return os<<(std::string)p;
}

///A locus is a set of points
class Locus{
   private:
      ///For each WeightType we store each point's value so this is
      ///number of WeightTypes this Locus knows about by NPoints array
      std::map<WeightType,std::vector<double> > Weights_;

      ///A 3 by NPoints block of memory for the Cartesian components
      std::vector<double> Carts_;

      ///A NPoints long array of the Point instances
      std::vector<Point> Points_;

   public:
      ///Makes a Locus that contains memory for N points
      Locus(size_t N=0);

      ///Adds a point to the Locus
      void AddPoint(const Point& APoint);

      ///Returns the number of points currently in the Locus
      size_t size()const noexcept{return Points_.size();}

      ///Returns the I-th point
      Point operator[](size_t i)const;

      ///Returns the I-th component of the J-th point
      ///(by reference so you can get the address)
      const double& operator()(size_t I,size_t J)const;
};

///Passes each point to the ostream
inline std::ostream& operator<<(std::ostream& os,const Locus& l){
   for(size_t i=0;i<l.size();++i)os<<l[i]<<std::endl;
   return os;
}


}}/* End namespaces */

#endif /* BPMODULE_MATH_LOCUS_HPP_ */
