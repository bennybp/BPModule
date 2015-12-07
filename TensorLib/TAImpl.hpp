#ifndef TAIMPL_HPP_
#define TAIMPL_HPP_

#include "tiledarray.h"
#include "Shape.hpp"
extern TiledArray::World* world;

namespace TensorWrap{

/*! \brief Implements the tensor guts for Tiled Array
 *
 * This is a glorified struct of typedefs basically
 *
 */
template<typename Data_t,size_t Rank>
class TAImpl{
   public:

      ///The main tensor type
      typedef typename TiledArray::Array<Data_t,Rank> TensorBase_t;

      ///The indexed tensor type
      typedef typename TiledArray::expressions::TsrExpr<TensorBase_t>
         IndexedTensor_t;

      ///Wraps the constructor
      static TensorBase_t* Make(const Shape& ShapeIn){
         return new TensorBase_t(*world,MakeRanges(ShapeIn));
      }

      ///Fills this processor's elements with the given value
      static void Fill(const Data_t& value, TensorBase_t& ATensor){
         ATensor.fill_local(value);
      }

      ///Converts TensorBase to indexed tensor
      static IndexedTensor_t DeRef(const char* string,
                                   TensorBase_t& ATensor){
         return ATensor(string);
      }


      ///Prints the tensor out
      static std::ostream& PrintOut(std::ostream& os,
                                    const TensorBase_t& ATensor){
         os<<ATensor;
         return os;
      }

      ///Returns the element with the given coordinate index
      template<size_t Rank2>
      static TensorBase_t* Get(const std::array<size_t,Rank2>& Idx,
            TensorBase_t& ATensor){

         //TARng_t Range=MakeRanges(ATensor.range().upbound(),
          //                        ATensor.range().lobound(),Rank2)
         //TensorBase_t::value_type ATensor.get(Idx);

      }
   private:

      ///A typedef of TA's one-dimensional range object
      typedef TiledArray::TiledRange1 TARng1_t;

      ///A typedef of TA's range object
      typedef TiledArray::TiledRange TARng_t;

      /*! \brief Given a shape, returns a vector of each rank's ranges
       *
       *   For a tensor of rank \f$N\f$ TA expects an object of type
       *   TARng_t that has \f$N\f$ objects of type TARng1_t such that
       *   each object contains the block boundaries of that rank.  For
       *   example, assume we have a 16 by 16 matrix broken up into
       *   16 4 by 4 blocks, we need to create a vector that looks like:
       *   \f[
       *      \left\lbrace
       *        \left\lbrace 4,8,12,16\right\rbrace,
       *        \left\lbrace 4,8,12,16\right\rbrace
       *       \right\rbrace,
       *   \f]
       *   where the inner braces are the TARng1_t objects.
       *
       *   For the time being our returned range looks like:
       *   \f[
       *      \left\lbrace
       *        \left\lbrace 0,L_1\right\rbrace,
       *        \left\lbrace 0,L_2\right\rbrace,
       *        \cdots,
       *        \left\lbrace 0,L_N\right\rbrace
       *       \right\rbrace,
       *   \f]
       *   where \f$L_i\f$ is the length of rank i.
       */
      template<typename T2>
      static TARng_t MakeRanges(const T2& ShapeIn){
         std::cout<<ShapeIn<<std::endl;
         std::vector<TARng1_t> Range;
         for(size_t r=0;r<ShapeIn.size();++r)
            Range.push_back(TARng1_t(ShapeIn[r].cbegin(),ShapeIn[r].cend()));
         return TARng_t(Range.begin(),Range.end());
      }

      ///An iterator to a TA tensor
      class TAItr{
         public:
            ///Moves to next coordinate index
            TAItr& operator++(){
               bool Changed=false;
               for(size_t r=Rank;r>0 && !Changed;--r){
                  if(Index_[r-1]<Finish_[r-1]){
                     ++Index_[r-1];
                     if(Index_[r-1]<Finish_[r-1])Changed=true;
                     if(r!=Rank&&Changed)
                        for(size_t s=r;s<Rank;++s)
                           Index_[s]=Start_[s];
                  }
               }
               if(!Changed){
                  *TileI_=Tile_;
                  ++TileI_;
                  if(TileI_!=Parent_.end()){
                     Tile_=Parent_.trange().make_tile_range(TileI_.ordinal());
                     Start_=Tile_.range().lobound();
                     Finish_=Tile_.range().upbound();
                  }
               }
               return *this;
            }

            ///Returns the spot where you should put your data
            Data_t& operator*(){
               return Tile_[Tile_.range().ordinal(Index_)];
            }

            ///True if iterators equal each other (defer's to TA's iterators)
            bool operator==(const TAItr& other){
               return (TileI_==other.TileI_); //&& (Index_==other.Index_);
            }

            ///Returns true if two iterators are different Negates operator==()
            bool operator!=(const TAItr& other){return !(*this==other);}
            std::ostream& operator<<(std::ostream& os){
               for(size_t i=0;i<Rank;i++)os<<Index_[i]<<" ";
               os<<std::endl;
               return os;
            }
         private:
            friend TAImpl<Data_t,Rank>;
            TAItr(TensorBase_t& Tensor,bool AtEnd):
               Parent_(Tensor),
               TileI_(!AtEnd?Tensor.begin():Tensor.end()),
               TileEnd_(Tensor.end()),
               Tile_(Tensor.begin()!=Tensor.end()?
                     Tensor.trange().make_tile_range(Tensor.begin().ordinal()):
                     TensorBase_t::value_type()),
               Start_(Tile_.range().lobound()),
               Finish_(Tile_.range().upbound()),
               Index_({}){

            }
            TensorBase_t& Parent_;
            typename TensorBase_t::iterator TileI_;
            typename TensorBase_t::const_iterator TileEnd_;
            typename TensorBase_t::value_type Tile_;
            std::vector<size_t> Start_,Finish_;
            std::array<size_t,Rank> Index_;
      };

   public:
      typedef TAItr iterator;
      //typedef TAItr const_iterator;
      static iterator begin(TensorBase_t& ATensor){
         return TAItr(ATensor,false);
      }
      static iterator end(TensorBase_t& ATensor){
         return TAItr(ATensor,true);
      }
};

}//End namespace TensorWrap

template<typename Data_t, size_t Rank>
inline std::ostream& operator<<(std::ostream& os, 
      const typename TensorWrap::TAImpl<Data_t,Rank>::TAItr& itr){
   itr<<os;
   return os;
}



#endif /* TAIMPL_HPP_ */
