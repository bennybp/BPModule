/** 
 * @file Locus.cpp
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
#include <sstream>
#include "bpmodule/math/Locus.hpp"
#include "bpmodule/exception/Assert.hpp"
#include "bpmodule/exception/Exceptions.hpp"

namespace bpmodule{
namespace math{

Point::Point(double x,double y,double z):
      Carts_(new double[3]{x,y,z}),WeOwnCarts_(true){
}

Point::~Point(){
   if(WeOwnWeights_){
      std::map<WeightType,double*>::iterator WI=Weights_.begin(),
            WEnd=Weights_.end();
      for(;WI!=WEnd;++WI)
         if(WI->second!=nullptr){
            delete WI->second;
            WI->second=nullptr;
         }
   }
   if(WeOwnCarts_&&Carts_!=nullptr){
      delete[] Carts_;
      Carts_=nullptr;
   }
}

void Point::AddWeight(const WeightType& WT,double w){
   if(Weights_.count(WT)==1)//Already have the memory for the weight
      *(Weights_[WT])=w;
   else if(Locus_==nullptr){//We own the weight for the moment
      Weights_[WT]=new double{w};
      WeOwnWeights_=true;
   }
   else{//New type of weight (yay!)

   }
}

double Point::Weight(const WeightType& WT)const{
   exception::Assert<exception::ValueOutOfRange>(
      Weights_.count(WT)!=1,
      "Requested weight not set","Weight",WT);
   exception::Assert<exception::ValueOutOfRange>(
      Weights_.at(WT)!=nullptr,"Value of weight not set yet!!!"
   );
   return *(Weights_.at(WT));
}

double Point::operator[](size_t I)const{
   exception::Assert<exception::ValueOutOfRange>(
      I<3,"Requested component is not in range [0,3)",
      "Component",I
   );
   exception::Assert<exception::ValueOutOfRange>(
      Carts_==nullptr,"Cartesian coordinates are not set yet!!!"
   );
   return Carts_[I];
}

Point::operator std::string()const{
   std::stringstream ss;
   ss<<(*this)[0]<<" "<<(*this)[1]<<" "<<(*this)[2];
   return ss.str();
}


/****************************/
//    Begin Locus Defs
/****************************/

//Adds a value to a vector, returns true if it reallocated
bool LoadVector(std::vector<double>& vec,double value2add){
   //How many elements can we originally hold
   size_t OrigCap=vec.capacity();
   vec.push_back(value2add);
   return vec.capacity()!=OrigCap;
}

Locus::Locus(size_t N):Carts_(3*N,0.0){

}

void Locus::AddPoint(const Point& APoint){
   //Number of points we currently have and max we can support
   size_t NP=size(),NMax=Carts_.size()/3;

   //One way or another we are adding this point
   //We now have NP+1 points
   Points_.push_back(Point());

   //Iterators to the point's weights
   std::map<WeightType,double*>::const_iterator Itr=APoint.Weights_.begin(),
         ItrEnd=APoint.Weights_.end();

   //Did either the carts or weights vectors reallocate?
   bool ReAll=false;

   //Copy the carts over
   for(size_t i=0;i<3;i++){
      if(NP<NMax)Carts_[NP*3+i]=APoint[i];
      else ReAll=ReAll || LoadVector(Carts_,APoint[i]);
   }

   //If Carts reallocated refresh pointers
   for(size_t i=0;i<NP&&ReAll;i++)
      Points_[i].Carts_=&(Carts_[i*3]);
   //Finally make sure our new pointer is set
   Points_.back().Carts_=&(Carts_[NP*3]);

   //Now for the weights
   ReAll=false;
   for(;Itr!=ItrEnd;++Itr){
      double value=*(Itr->second);
      if(Weights_.count(Itr->first)!=1){//WeightType DNE, add it
         Weights_[Itr->first]=std::vector<double>(NMax>NP?NMax:NP+1,0.0);
         ReAll=true;
         //Set this point's weight
         Weights_[Itr->first][NP]=value;
      }
      else{
         if(NP<NMax)Weights_[Itr->first][NP]=value;
         else ReAll=LoadVector(Weights_[Itr->first],value);
      }
      if(ReAll)
         for(size_t i=0;i<NP;i++)
            Points_[i].Weights_[Itr->first]=&(Weights_[Itr->first][i]);
      Points_[NP].Weights_[Itr->first]=&(Weights_[Itr->first][NP]);
   }
}

Point Locus::operator[](size_t I)const{
   exception::Assert<exception::ValueOutOfRange>(
         I<size(),
         "Requested point not in the range [0,size)",
         "Point",I
   );
   return Points_[I];
}

const double& Locus::operator()(size_t I,size_t J)const{
   exception::Assert<exception::ValueOutOfRange>(
         I<size(),
         "Requested point not in the range [0,size)","Point",I);
   exception::Assert<exception::ValueOutOfRange>(
         J<3,
         "Requested Cartesian coordinate not in range [0,3)","Component",J);
   return Carts_[I*size()+J];
}

}}//End namespaces
