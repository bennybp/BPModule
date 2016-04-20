/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CombItr.hpp
 * Author: richard
 *
 * Created on March 17, 2016, 10:52 AM
 */

#ifndef PULSAR_GUARD_MATH__COMBITR_HPP_
#define PULSAR_GUARD_MATH__COMBITR_HPP_

#include <vector>
#include "pulsar/exception/Exceptions.hpp"
namespace pulsar{
namespace math{

/** \brief An iterator to generate all combinations from n objects,
 *         k at a time
 *
 *  A combination is an unordered set, which is math speak for
 *  the combination {1,2,3} equals {2,3,1}, i.e. the order of the
 *  elements do not matter.  This differs from a permutation, where
 *  the order does matter (and the permutation {1,2,3} does not equal
 *  the permutations {2,3,1} (to be more better ats da math, I should
 *  be using "()"'s to denote an ordered set, but f-it)).  If you
 *  need permutations there is a C++ function std::next_permutation
 *  in the <algorithm> header that probably does what you want.
 *
 *  Anyways, this iterator generates all unique combinations of the
 *  elements in some input set.  The combinations come back in so
 *  called lexical order (generalization of alphabetical order to
 *  arbitrary sets), where the order is that
 *  of the elements in your input set, not their values (i.e. if you
 *  gave the set {2,3,1} and wanted all pairs you would get:
 *  \verbatim
 *  2 3 (0 1)
 *  2 1 (0 2)
 *  3 1 (1 2)
 *  \endverbatim
 *  where the values in parentheses are the indices.  Note that this
 *  is distinctly different than:
 *  \verbatim
 *  1 2
 *  1 3
 *  2 3
 *  \endverbatim
 *  which is lexical order of the elements in that set).
 *
 *
 *  The actual implementation of this is really nasty if you are not
 *  comfortable with STL container iterator arithmetic.  Basically
 *  what happens is we store 2K iterators the first K are iterators
 *  to our current elements, the second K are the stopping spots
 *  of each element.  Starting from the right of our first K we
 *  find the first iterator we can increment by seeing which one
 *  doesn't equal its ending iterator upon being increased.  Once we
 *  find it, we set all iterators higher than it so that we are in a
 *  valid combination.
 *
 *  If that's too technical, here's all you need to know you can give
 *  me any STL container that is forward and reverse iteratable (which
 *  is almost all of them).  For simplicity let's assume you gave
 *  me a vector of integers:
 *  \code
 *  //The type of this iterator
 *  typedef CombItr<std::vector<int> > CombItr_t;
 *
 *  //Declare a set of integers
 *  std::vector<int> MySet;
 *
 *  //Fill it
 *  for(size_t i=0;i<5;i++)MySet.push_back(i);
 *  //Set now contains: {0,1,2,3,4}
 *
 *  //Make a combination iterator that will generate all
 *  //combinations from our set taken K at a time
 *  int K=3;
 *  CombItr_t MyItr(MySet,K);
 *
 *  //Now loop
 *  for(;!MyItr.Done();++MyItr){
 *      for(size_t i=0;i<K;i++)
 *         std::cout<<(*MyItr)[i]<<" ";
 *      std::cout<<std::endl;
 *  }
 *  \endcode
 *
 *  The output should be:
 *  \verbatim
    0 1 2
    0 1 3
    0 1 4
    0 2 3
    0 2 4
    0 3 4
    1 2 3
    1 2 4
    1 3 4
    2 3 4
    \endverbatim

    Again, nearly every STL container should work.  It is worth
    noting however, that the combination you get back is of the
    same type of as the container you gave me.  So, if you gave
    me an std::vector of matrices, you will get combinations
    back that are std::vectors of matrices.  Hence, I strongly
    recommend that your objects copy quickly, i.e. you should
    probably be giving me plain old data types, pointers, or
    iterators....
  
    \todo I wrote this awhile ago, should probably be C++11-ified

    \param T The type of the set you want the combinations of.
 */
template <typename T>
class CombItr {
   private:
      ///The current combination
      T Comb_;
      ///The number of elements in each combination
      size_t K_;
      ///Typedefs to keep my sanity...
      typedef typename T::const_iterator TItr_t;
      typedef std::vector<TItr_t> Index_t;
      ///The current iterator
      Index_t Indices_;
      ///Where each index stops
      Index_t End_;
      ///The set the user gave us
      const T& Set_;
      ///Are we done?
      bool Done_;
      ///Sets Comb_ to the next combination
      void Next();
   public:
      ///Makes a new iterator that will run through Set, K at a time
      CombItr(const T& Set, size_t K);
      ///Returns the combination this iterator points to
      const T& operator*()const{return Comb_;}
      ///Returns true if we have run through all combinations
      bool Done()const{return Done_;}
      ///Allows access to the member functions of your container
      const T* operator->()const{return &Comb_;}
      ///Increments the iterator
      const CombItr<T>& operator++(){Next();return *this;}
};

/*********** Implementations ***************/
template <typename T>
CombItr<T>::CombItr(const T& Set, size_t K) :
      K_(K), End_(K),Set_(Set), Done_(false) {
   if (Set.size()<K)
    throw pulsar::exception::GeneralException(
           "I don't know how to generate combinations with"
           " more items than you gave me....");

   ///Form the starting combination
   TItr_t ElemI=Set.begin();
   while (Indices_.size()<K) {
      Indices_.push_back(ElemI);
      Comb_.insert(Comb_.end(), *ElemI);
      ++ElemI;
   }
   ///Get the ending iterator for each index
   typedef typename T::const_reverse_iterator rTItr_t;
   rTItr_t ElemJ=Set.rbegin();
   typedef typename Index_t::reverse_iterator rItr_t;
   rItr_t EndI=End_.rbegin(),EndEnd=End_.rend();
   for(;EndI!=EndEnd;++EndI,++ElemJ)
      (*EndI)=ElemJ.base();
}

template <typename T>
void CombItr<T>::Next() {
   //Loop from the end of our indices backwards looking for one
   //we can increment
   typedef typename Index_t::reverse_iterator rItr_t;
   typedef typename Index_t::iterator Itr_t;
   rItr_t It=Indices_.rbegin(),ItEnd=Indices_.rend(),
         EndIt=End_.rbegin();
   bool reset=false;
   for (; It!=ItEnd; ++It,++EndIt) {
      //Increment the iterator It is pointing to, not It!!!!
      ++(*It);
      //Check if that increment was good
      if (*It!=*EndIt)break;
      //If not, we will need to reset all indices right of this one
      reset=true;
   }
   if (It==ItEnd) {//We ran out of indices...
      Done_=true;
      return;
   }
   else if (reset) {
      //Increment our reverse iterator once so the forward iterator
      //returned by it is at the same location (silly reverse to
      //forward semantics...)
      ++It;
      Itr_t It2=It.base(),It2End=Indices_.end();
      while(true){//Reset being tripped guarantees us one loop
         TItr_t TempItr=(*It2);
         ++It2;
         if(It2==It2End)break;
         (*It2)=TempItr;
         ++(*It2);
      }
   }
   //Finally fill in Combination
   Itr_t ElemI=Indices_.begin(),ElemEnd=Indices_.end();
   T Temp;
   for(;ElemI!=ElemEnd;++ElemI)Temp.insert(Temp.end(),*(*ElemI));
   Comb_=Temp;
}

}}//End namespaces
#endif /* COMBITR_HPP */

