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
 * \todo When slicing and dicing I update my shapes, TA keeps
 * the original shape, i.e. if we carve out the region that is
 * [4,8) by [2,6) our result is 3 rows by 3 columns.  I remap this
 * to [0,4) by [0,4), but TA still shows [4,8) by [2,6).
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
      static TensorBase_t* Make(const Shape<Rank>& ShapeIn){
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
         /* This maps to TA's operator<<() on line 614 of array.h.
          * The first thing this does is grabs the number of tiles
          * in the tensor.  Next it loops over all of the tiles in that
          * tensor, and if they are not zero, retrieves them via the
          * function find(x) where x is the tile number.  Find checks
          * to ensure the index is in range, and then calls
          * ArrayImpl::get(x), which then calls DistributedStorage::get(x).
          * This then immediately returns a future if the tile is local, or
          * it gets the tile if it's remote.
          *
          */
         os<<ATensor;
         return os;
      }

      ///Given a set of offsets fills a tensor
      template<size_t Rank>
      static TensorBase_t*
         Get(const std::set<size_t>& Idx,
             const Shape<Rank>& OldShape,
             TensorBase_t& ATensor){
         Shape<Rank> NewShape=OldShape.SubShape(Idx);
         TensorBase_t* NewT=Make(NewShape);
         //Need random access to set
         std::vector<size_t> IdxRdm(Idx.begin(),Idx.end());
         typename TensorBase_t::iterator TI(NewT->begin()),TIend(NewT->end());
         //TI is an iterator to the local tiles I need to fill
         for(;TI!=TIend;++TI)
            *TI=ATensor.find(IdxRdm[TI.ordinal()]);
         return NewT;
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
      static TARng_t MakeRanges(const Shape<Rank>& ShapeIn){
         std::vector<TARng1_t> Range;
         for(size_t r=0;r<Rank;++r)
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
                  *(*TileI_)=Tile_;
                  ++(*TileI_);
                  if((*TileI_)!=Parent_.end()){
                     Tile_=Tile_t(Parent_.trange().make_tile_range(TileI_->ordinal()));
                     Start_=Tile_.range().lobound();
                     Finish_=Tile_.range().upbound();
                     for(size_t i=0;i<Rank;++i)
                        Index_[i]=Start_[i];
                  }
               }
               return *this;
            }

            ///Returns the index
            const std::array<size_t,Rank>& Index()const{return Index_;}

            ///Returns the spot where you should put your data
            Data_t& operator*(){
               return Tile_[Tile_.range().ordinal(Index_)];
            }

            ///True if iterators equal each other (defer's to TA's iterators)
            bool operator==(const TAItr& other){
               return ((*TileI_)==(*other.TileI_)); //&& (Index_==other.Index_);
            }

            ///Returns true if two iterators are different Negates operator==()
            bool operator!=(const TAItr& other){return !(*this==other);}
            std::ostream& operator<<(std::ostream& os){
               for(size_t i=0;i<Rank;i++)os<<Index_[i]<<" ";
               os<<std::endl;
               return os;
            }
         private:
            typedef typename TensorBase_t::iterator Itr_t;
            typedef typename TensorBase_t::const_iterator cItr_t;
            typedef typename TensorBase_t::value_type Tile_t;
            static Itr_t* GetBegin(TensorBase_t& Tensor){
               return new Itr_t(Tensor.begin());
            }

            static Itr_t* GetEnd(TensorBase_t& Tensor){
               return new Itr_t(Tensor.end());
            }

            static Tile_t MakeTile(TensorBase_t& Tensor,
                                   std::shared_ptr<Itr_t> TileI,bool AtEnd){
               if(AtEnd||(*TileI)==Tensor.end())return Tile_t();
               return Tile_t(Tensor.trange().make_tile_range(
                     TileI->ordinal()));
            }

            friend TAImpl<Data_t,Rank>;
            TAItr(TensorBase_t& Tensor,bool AtEnd):
               Parent_(Tensor),
               TileI_(!AtEnd?GetBegin(Tensor):GetEnd(Tensor)),
               TileEnd_(GetEnd(Tensor)),
               //Tensor.trange() returns the object holding the tensor's
               //tile range, which is of type TensorImpl<Policy>::trange_type
               //TileI_.ordinal() returns the tile's index
               Tile_(MakeTile(Tensor,TileI_,AtEnd)),
               Start_(Tile_.range().lobound()),
               Finish_(Tile_.range().upbound()),
               Index_({}){
               for(size_t i=0;i<Rank&&Start_.size()==Rank;++i)
                  Index_[i]=Start_[i];
               std::cout<<"Iterator made\n";


            }
            TensorBase_t& Parent_;
            /** \brief An iterator over the tiles in the parent tensor
             *
             *  Internal to TA, this maps to impl_type::iterator,
             *  which maps to ArrayIterator<ArrayImpl_,reference>
             */
            std::shared_ptr<Itr_t> TileI_;
            ///An iterator at the last tile in parent tensor
            std::shared_ptr<Itr_t> TileEnd_;
            Tile_t Tile_;
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
