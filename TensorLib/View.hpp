#ifndef TENSORLIB_VIEW_HPP_
#define TENSORLIB_VIEW_HPP_
#include <array>
#include <string>
#include <vector>
#include <set>
#include <map>
#include "Shape.hpp"

namespace TensorWrap{

/** \brief Keeps track of how we are viewing the tensor at the moment
 *
 *  Given a shape, we know how many blocks we have and what rank the
 *  underlying data is.  If we have a rank \f$R\f$ tensor and \f$r_i\f$
 *  partitions along the \f$i\f$-th rank, we have a maximum of
 *  \f$\prod_ir_i\f$ blocks where each block is also of rank \f$R\f$.
 *  For simplicity we assume that these blocks are all sitting in some
 *  vector and the goal will be to map our current view to one block.
 *
 *  The initial view is trivial to choose a block, if we want the
 *  (i,j,...,R)-th
 *  block we return \f$\sum_{k=0}^{R-1}I_k\prod_{j=k+1}^{R-1}r_j\f$,
 *  which is just the generalization of row-major to a rank \f$R\f$ tensor.
 *  If the first dimension is a faux rank 2, so that the block we want
 *  is now the (l,m,j,...,R)-th block we can map l and m back to i then
 *  return the block.  This strategy continues for each index.
 *
 *  As an example, consider the MO coefs arranged AO spin orbitals
 *  by MO spin orbitals.  This is a rank 2 tensor.  Inside each rank
 *  we have sorted the orbitals by spin, and then within the MO rank we
 *  have further sorted by occupation.  If dimension 1 goes alpha, beta and
 *  dimension 2 goes alpha occupied, alpha virtual, beta occupied, beta
 *  virtual, we could request the alpha virtual block by:
 *  \code
 *  T('a','a','v');
 *  \endcode
 *  Under the hood this maps to the (0,0*2+1)-th block, which in turn maps
 *  to 0*4+(0*2+1).
 *
 *
 */
template<size_t Rank>
class View{
   private:
      typedef std::vector<size_t> Length_t;
      typedef std::map<std::string,size_t> Label_t;
   public:
      ///Makes a view where each rank is indexed by block
      View(const Shape<Rank>& AShape);

      ///Makes an unusable view until Fill is called
      View()=default;


      /** \brief Makes rank I look like it's Idx[0] by Idx[1] by Idx[2] by...
      *
      *   Restrictions on arguments:
      *     Names can't contain the null string
      *     Given that you have r_i partitions you can't have more labels
      *     than that, so:
      *     1. NewRank<=r_i
      *     2. Sum of Idx must be <=r_i
      *     3. R2<=
      *
      *   \param[in] I The dimension we are changing
      *   \param[in] Names What the new dimensions are called
      *   \param[in] Length How long each one of the new dimensions is
      */
      void ChangeView(size_t I,
            const std::vector<std::string>& Names,
            const std::vector<size_t>& Length);

      /** \brief Given a mapped block's coordinate index returns the
       *   requested tensor.
       *
       *
       *   Say we have an apparent rank of \f$R\f$, the user may
       *   give us from 0 to \f$R\f$ labels, which we call \f$n\f$
       *   and we have to then return a \f$R-n\f$ rank tensor made
       *   from the requested blocks.  The easiest cases are afully
       *   specified block, that is \f$n=R\f$, and when \f$n=0\f$ the
       *   whole tensor.  For all other scenarios we need to return
       *   return a set of blocks in row-major form.
       *
       *   We have a lot of cases to worry about, for
       *   an apparent rank 3 tensor, the user can give us:
       *   \code
       *   //Block abc
       *   T("a","b","c");
       *
       *   //The ab row
       *   T("a","b");
       *
       *   //Also fine
       *   T("a","b","");
       *
       *   //The b matrix
       *   T("","b");
       *
       *   //Also fine
       *   T("","b","");
       *
       *   //The entire tensor
       *   T("","","");
       *   \endcode
       *
       *   Our present strategy is to expand whatever was given to us
       *   into an index that is rank \f$R\f$ long.  In the code example
       *   above this is already present for scenarios 1,3,5, and 6.
       *   For scenario 2, we make it look like scenario 3, and for
       *   scenario 4 we make it look like 5.  We then loop over all
       *   indices filling an std::set with the blocks to retrieve.  Any
       *   time a null string is encountered we retrieve all blocks that
       *   index maps to.
       *
       */
      template<typename... Args>
      std::set<size_t> operator()(Args... args)const;

      void Fill(const Shape<Rank>& DaShape);

      std::ostream& operator<<(std::ostream& os)const;

   private:

      ///Returns the fake rank of real dimension I
      size_t RankI(size_t I)const{return Labels_[I].size();}

      ///Returns the length of fake dimension J inside real dimension I
      size_t LengthIJ(size_t I,size_t J)const{return Labels_[I][J].size();}

      ///Maps from coordinate index to offset
      template<typename T,typename T2>
      size_t Offset(T Prods, T2 Idx)const;

      void Recurse(size_t RRank,size_t Pi,
            std::array<std::vector<size_t>,Rank>& ABlock,
            std::set<size_t>& Blocks,
            const std::array<std::vector<std::string>,Rank>& Idx)const;

      /** \brief The original number of elements offset
       *
       *  Assume we have a rank 3 tensor with \f$r_1\f$,\f$r_2\f$,
       *  and \f$r_3\f$ partitions along each dimension.  We are
       *  thinking of this as row-major so Prod[0]=\f$r_2 r_3\f$,
       *  Prod[1]=\f$r_3\f$, and Prod[1]=1. That is each element
       *  of this array is the total size of the remaining ranks.
       *  This is needed when going from coordinate index to
       *  offset.
       */
      std::array<size_t,Rank> Prod_;

      ///The number of partitions per rank
      std::array<size_t,Rank> NParts_;

      ///The overall pretend rank
      size_t FauxRank_=0;

      ///Similar to Prod_, except for our current view
      std::array<Length_t,Rank> Lengths_;

      ///The labels of the partitions by rank
      std::array<std::vector<Label_t>,Rank> Labels_;

};

template<size_t Rank>
View<Rank>::View(const Shape<Rank>& AShape){
   Fill(AShape);
}

template<size_t Rank>
void View<Rank>::Fill(const Shape<Rank>& AShape){
   FauxRank_=0;
   //Loop backwards for the product to work out
   for(size_t i=Rank;i>0;--i){
      size_t il1=i-1;
      NParts_[il1]=AShape.NParts(il1);
      Labels_[il1]=std::vector<Label_t>(1,{{"",0}});
      FauxRank_++;
      Prod_[il1]=(i==Rank?1:Prod_[i]*NParts_[i]);
   }
}

template<size_t Rank>
void View<Rank>::ChangeView(size_t I,
      const std::vector<std::string>& Names,
      const std::vector<size_t>& Lengths){
   //Update our overall apparent rank
   //Needs to be done in two steps b/c we can't handle a negative value
   size_t NewRank=Lengths.size();
   FauxRank_-=RankI(I);
   FauxRank_+=NewRank;

   //Update the I-th dimension's labels and subdimensions
   //This line sets RankI()
   Labels_[I]=std::vector<Label_t>(NewRank);
   Lengths_[I]=Length_t(NewRank,1);
   for(size_t i=0,counter=0;i<Lengths.size();++i){
      size_t Ri=NewRank-i;
     if(i!=0)
        Lengths_[I][Ri-1]=Lengths_[I][Ri]*Lengths[Ri];
     for(size_t j=0;j<Lengths[i];++j,++counter)
        Labels_[I][i][Names[counter]]=j;
   }
}
template<size_t Rank>
void View<Rank>::Recurse(size_t RRank,size_t Pi,
      std::array<std::vector<size_t>,Rank>& ABlock,
      std::set<size_t>& Blocks,
      const std::array<std::vector<std::string>,Rank>& Idx)const{
   if(RRank==Rank){//End of recursion
      //This will be the actual block we want
      std::array<size_t,Rank> RealIndex;

      for(size_t i=0;i<Rank;i++)
         RealIndex[i]=Offset(Lengths_[i],ABlock[i]);

      Blocks.insert(Offset(Prod_,RealIndex));
   }
   else{
      const std::string& Index=Idx[RRank][Pi];
      bool IsNull=(Index=="");

      //If the current index is NULL we need to loop over it
      for(size_t k=0;k<(IsNull?LengthIJ(RRank,Pi):1);k++){
         //Dereference any user supplied index
         ABlock[RRank][Pi]=(IsNull?k:Labels_[RRank][Pi].at(Index));
         //Move to the next index along this real dimension (if possible)
         if(Pi+1<RankI(RRank))
            Recurse(RRank,Pi+1,ABlock,Blocks,Idx);
         //Move to the next real index
         else
            Recurse(RRank+1,0,ABlock,Blocks,Idx);
      }
   }
}



template<size_t Rank>
template<typename... Args>
std::set<size_t> View<Rank>::operator()(Args... args)const{
   const size_t RankGiven=sizeof...(args);
   std::vector<std::string> Temp({args...});

   //Will be our current index in a nested form
   std::array<std::vector<size_t>,Rank> ABlock;

   //Will be the symbolic names of the current index in a nested from
   std::array<std::vector<std::string>,Rank> Idx;

   //Fully expand the index requested by the user so there are no blanks
   //and zero out ABlock
   for(size_t i=0,counter=0;i<Rank;i++){
      ABlock[i]=std::vector<size_t>(RankI(i),0);
      Idx[i]=std::vector<std::string>(RankI(i),"");
      for(size_t j=0;j<RankI(i)&&counter<Temp.size();j++,counter++)
         Idx[i][j]=Temp[counter];
   }

   //These are the blocks of the full tensor, in row-major form
   //that go with the requested index
   std::set<size_t> DaBlocks;

   Recurse(0,0,ABlock,DaBlocks,Idx);
   return DaBlocks;

}

template<size_t Rank>
template<typename T, typename T2>
size_t View<Rank>::Offset(T Prod, T2 Idx)const{
   size_t Result=0;
   for(size_t i=0;i<Prod.size();++i)Result+=Idx[i]*Prod[i];
   return Result;
}

template<size_t Rank>
std::ostream& View<Rank>::operator<<(std::ostream& os)const{
   os<<"Overall Rank: "<<Rank<<" mapped to: "<<FauxRank_
     <<" ranks"<<std::endl;
   for(size_t i=0;i<Rank;i++){
      os<<"Rank: "<<i<<" is : "<<RankI(i)<<" with names: {";
      for(size_t j=0;j<RankI(i);++j){
         Label_t::const_iterator LI=Labels_[i][j].begin(),
               LEnd=Labels_[i][j].end();
         os<<"{";
         for(;LI!=LEnd;++LI)os<<LI->first<<" ";
         os<<"}";
      }
      os<<"}"<<std::endl;
   }
   return os;
}


}//End namespace TensorWrap

template<size_t Rank>
std::ostream& operator<<(std::ostream& os,const TensorWrap::View<Rank>& V){
   return V<<os;
}

#endif /* TENSORLIB_VIEW_HPP_ */
