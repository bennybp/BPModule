#ifndef TENSORLIB_SHAPE_HPP_
#define TENSORLIB_SHAPE_HPP_
#include<array>

namespace TensorWrap {

/*! \brief A class to keep track of the dimensions of our tensor
 *
 *   Given an initial tensor A, of rank R, for which the \f$i\f$-th
 *   dimension is of length \f$A_i\f$, \f$i\f$ runs from \f$[0, N_i)\f$.
 *   Things get more complicated when we start ascribing meanings to
 *   subspaces.  There are two main ways a user can create subspaces.
 *   At Tensor creation, the user may give us the length of each block
 *   along a dimension, e.g. if the first \f$no\f$ elements are the number
 *   of occupied orbitals and the second \f$nv\f$ elements are the number
 *   of virtual orbitals, the user provides \f$\lbrace no,nv\f$, but
 *   our shape needs to look like: \f$[0,no)[no,no+nv)\f$.
 *   The other way a user may build subspaces is via direct summation,
 *   this means we are adding two Shape instances.
 *
 *  Default construction of a shape is a scalar embedded in a rank R
 *  vector space.  As ranks are added (you can't subtract) this
 *  class adjusts to the changes and ensures all the bookkeeping is
 *  done for us.
 *
 *
 *  \param Rank The rank of the tensor associated with this class
 */
template<size_t Rank>
class Shape {
   private:
      ///Typedef of the type of this class
      typedef Shape<Rank> My_t;

      ///Typedef of the inializer list
      template <typename T>
      using IL=std::initializer_list<T>;
   public:
      ///Calls fill to initialize the Shape
      Shape(const std::array<size_t, Rank>& Idx) {Fill(Idx);}

      ///Creates a more complicated shape
      Shape(const IL<IL<size_t> >& Idx){
         Shape_.fill(Boundary_t(1,0));
         Initialize(this->Shape_,Idx);
      }

      ///Makes a shape, which is unusable until filled
      Shape(){Shape_.fill(Boundary_t(1,0));}

      ///Copy of index structure
      Shape(const My_t&)=default;

      ///Combines this and Other into this, returns this
      My_t& operator+=(const My_t& Other) {
         Initialize(this->Shape_,Other.Shape_,false);
         return *this;
      }

      ///Combines this and Other's ranges, returns result
      My_t operator+(const My_t& Other)const{
         return (Shape(*this))+=Other;
      }

      ///Sets the range of the i-th rank to [0,Idx[i])
      void Fill(const std::array<size_t, Rank>& Idx) {
         for (size_t i=0;i<Rank;++i)Shape_[i]=Boundary_t({0, Idx[i]});
      }

      ///Returns the rank of this shape
      size_t size()const{return Rank;}

      ///Returns the number of partitions along the i-th rank
      size_t NParts(size_t i)const{return (*this)[i].size()-1;}

      ///Returns the i-th index's range, invalidated by calling Fill
      const std::vector<size_t>& operator[](size_t i)const{return Shape_[i];}

      ///Allows the Shape to be printed
      std::ostream& operator<<(std::ostream& os)const{
         for(size_t i=0; i<Rank;i++){
            os<<"Rank: "<<i<<" [";
            size_t length=Shape_[i].size();
            for(size_t j=0;j<length;j++)
               os<<Shape_[i][j]<<(j!=length-1?",":")");
            os<<std::endl;
         }
         return os;
      }


      ///Given a series of blocks makes a shape consistent with it
      Shape<Rank> SubShape(const std::set<size_t>& Blocks)const{
         std::set<size_t>::const_iterator BI=Blocks.begin(),
               BIEnd=Blocks.end();

         Vec_t VecBlocks=Vectorize();
         Shape<Rank> Result;
         //Imagine we are adding two blocks from a row, if we always
         //add all dimensions than we will extend our layout too much
         //this array keeps track of that for us
         std::array<std::set<std::pair<size_t,size_t> >,Rank> AlreadyDone;
         for(;BI!=BIEnd;++BI){
            for(size_t j=0;j<Rank;j++){
               if(AlreadyDone[j].count(VecBlocks[*BI][j])==0){
               Result.Shape_[j].push_back(Result.Shape_[j].back()
                     +VecBlocks[*BI][j].second-VecBlocks[*BI][j].first);
                  AlreadyDone[j].insert(VecBlocks[*BI][j]);
               }
            }
         }
         return Result;
      }

   private:
      typedef std::vector<std::array<std::pair<size_t,size_t>,Rank> > Vec_t;

      /** \brief Takes the shape and flattens it so ranges can be looked
       *   up by offset
       *
       *   Assume we have a m by n by l tensor, broken up into mi, ni, and
       *   li partitions each.  We then have mi*ni*li rank 3 blocks.  Each
       *   block can be described by 6 indices, its start and stop indices
       *   along each of the three ranks.  Our goal in this function
       *   is to write out the length of the block's ranges in a
       *   vectorized format.
       *
       *   The returned format is the start and stop of the partition,
       *   indexed by block offset, then by the ranks of that block.
       *
       *
       */
      Vec_t Vectorize()const{
         Vec_t Result;
         std::vector<size_t> Idx(Rank,0);
         Recurse(0,Idx,Result);
         return Result;
      }

      ///The recursion necessary for Vectorize()
      void Recurse(size_t ri,std::vector<size_t>& Idx,Vec_t& Result)const{
         if(ri==Rank){//End recursion
            Result.push_back(std::array<std::pair<size_t,size_t>,Rank>());
            for(size_t i=0;i<Rank;++i)
               Result.back()[i]=(std::pair<size_t,size_t>(
                     (*this)[i][Idx[i]],(*this)[i][Idx[i]+1]));
         }
         else{
            for(size_t j=0;j<NParts(ri);j++){
               Idx[ri]=j;
               Recurse(ri+1,Idx,Result);
            }
         }
      }


      ///Fills in the shape object, UseBack=true is for the first case
      ///given in the class description, whereas false is the second
      template<typename T, typename T1>
      void Initialize(T& Add2Shape,const T1& Shape2Add,bool UseBack=true){
         typename T1::const_iterator I=Shape2Add.begin(),
               IEnd=Shape2Add.end();
         for(size_t i=0;I!=IEnd;++i,++I){
            Boundary_t& RankI=Add2Shape[i];
            size_t Last=RankI.back();
            typename T1::value_type::const_iterator J=I->begin(),
                  JEnd=I->end();
            for(;J!=JEnd;++J){
               if(*J!=0)RankI.push_back((UseBack?RankI.back():Last)+*J);
            }
         }
      }

      ///The object holding the boundaries of each rank
      typedef std::vector<size_t> Boundary_t;

      ///The type of our shape container
      typedef std::array<Boundary_t,Rank> Shape_t;

      ///The container that is holding all of the ranges
      Shape_t Shape_;
};

///Allows a shape to be passed to an ostream
template<size_t Rank>
inline std::ostream& operator<<(std::ostream& os,const Shape<Rank>& sha){
   return sha<<os;
}

} //End namespace TensorWrap

#endif /* TENSORLIB_SHAPE_HPP_ */
