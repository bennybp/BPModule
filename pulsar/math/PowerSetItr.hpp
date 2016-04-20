/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PowerSetItr.hpp
 * Author: richard
 *
 * Created on March 17, 2016, 6:07 PM
 */

#ifndef BPMODULE_GUARD_MATH__POWERSETITR_HPP_
#define BPMODULE_GUARD_MATH__POWERSETITR_HPP_
#include <memory>
#include "bpmodule/math/CombItr.hpp"
namespace bpmodule{
namespace math{
/** \brief Class to facilitate iterating over the power set of a set
 *
 *  Let's start with what a power set is.  Given a set \f$S\f$, the
 *  power set of \f$S\f$, usually denoted \f$\mathbb{P}(S)\f$ is the
 *  set of all subsets of \f$S\f$.  In particular this means the
 *  empty set and \f$S\f$ itself are in \f$\mathbb{P}(S)\f$.  In
 *  terms of combinatorics, there is a deep symmetry between the
 *  power set of \f$S\f$ and a row of Pascal's triangle, hence
 *  iterating over (at least part of) a power set typically arises
 *  when one is iterating over say all ways of picking one, two ,
 *  and three objects from a set.
 *
 *  Previously I had coded this iterator up in terms of counting in
 *  binary, which is based on the realization that the binary
 *  representation of \f$2^N-1\f$ contains \f$N\f$ 1's.  So counting
 *  from 0 to \f$2^N-1\f$, in binary will generate all of the
 *  \f$2^N\f$ subsets of \f$S\f$ if we associate a 1 in the \f$i\f$-th
 *  place with \f$S_i\f$ being in the current subset and a 0 with its
 *  absence.  Unfortunately this generates the subsets in a weird
 *  order, something akin to (assuming \f$S=\{1,2,3,\cdots,N\}\f$):
 *  \verbatim
 *  (empty set)
 *  1
 *  2
 *  1 2
 *  3
 *  1 3
 *  2 3
 *  1 2 3
 *  ...
 *  1 2 3...N
 *  \endverbatim
 *
 *  It's more natural to instead iterate in the order:
 *  \verbatim
 *  (empty set)
 *  1
 *  2
 *  3
 *  ...
 *  N
 *  1 2
 *  1 3
 *  ...
 *  1 2 3
 *  ...
 *  1 2 ...N
 *  \endverbatim
 *  this is the order the current generation loops in.
 *
 *  The implementation for this is straightforward, either by
 *  examining the intended order, or again by analogy to Pascal's
 *  triangle, one sees that we simply want to iterate over all
 *  combinations of our current set.  Hence we call CombItr \f$N\f$
 *  times.
 *
 *  Usage of this class is similar to its cousin class CombItr:
 *  \code
    ///Typedef of container
    typedef std::set<size_t> Set_t;

    ///Declare and fill a set of integers
    Set_t TestSet;
    for(size_t i=0;i<5;i++)TestSet.insert(i);

    ///Declare a power set iterator that runs over the whole set
    PowerSetItr<Set_t> MyItr(TestSet);

    ///Iterate until done
    for(;!MyItr.Done();++MyItr){
      Set_t::const_iterator elem=MyItr->begin(),elemEnd=MyItr->end();
      for(;elem!=elemEnd;++elem)
         std::cout<<*elem<<" ";
      std::cout<<std::endl;
   }
   \endcode
   The output should be:
   \verbatim

   0
   1
   2
   3
   4
   0 1
   0 2
   0 3
   0 4
   1 2
   1 3
   1 4
   2 3
   2 4
   3 4
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
   0 1 2 3
   0 1 2 4
   0 1 3 4
   0 2 3 4
   1 2 3 4
   0 1 2 3 4
   \endverbatim
 *
 * \todo This is an old class, C++11-ize it
 *
 * \param T The type of the set we are iterating over, will also be
 *          the type of susbsets returned by the iterator.
 */
template<typename T>
class PowerSetItr{
   private:
      ///The set we are generating the power set of
      const T& Set_;
      ///The maximum order this iterator is going up to
      size_t MaxOrder_;
      ///The first order we are considering
      size_t MinOrder_;
      ///The current order
      size_t Order_;
      typedef typename bpmodule::math::CombItr<T> CombItr_t;
      typedef typename std::shared_ptr<CombItr_t> SharedItr_t;
      SharedItr_t CurrentIt_;
      ///Have we iterated over the entire range yet?
      bool Done_;
      ///Implementation for getting the next element
      void Next();
   public:
      /** \brief Given a set, iterates over all subsets containing Min
       *      number of elements to Max number of elements
       *
       *  In an attempt to be mathematically rigorous the default is
       *  to iterate over the entire powerset, so the empty set to
       *  the entire set.  If your set has \f$N\f$ elements this
       *  corresponds to a Max of N and a Min of 0.  Specifying a
       *  negative number for either max or min will default to these
       *  values.  Note that this leads to a very odd scenario as
       *  far as C++ land is concerned, Max=2, for example, actually
       *  corresponds to choosing up to 2 elements and not choosing
       *  1 element.
       */
      PowerSetItr(const T& Set, int Min=-1, int Max=-1);
      ///Returns true if we have iterated over the whole range
      bool Done()const{return Done_;}
      ///Moves on to the next subset
      const PowerSetItr<T>& operator++(){Next();return *this;}
      ///Returns the current subset
      const T& operator*()const{return CurrentIt_->operator*();}
      ///Allows access of the subset's container's members
      const T* operator->()const{return CurrentIt_->operator->();}
};

/******************** Implementations ******************/
template<typename T>
PowerSetItr<T>::PowerSetItr(const T& Set, int Min,int Max):
        Set_(Set),
        MaxOrder_(Max<0?Set_.size():static_cast<size_t>(Max)),
        MinOrder_(Min<0?0:static_cast<size_t>(Min)),
        Order_(MinOrder_),
        CurrentIt_(new CombItr_t(Set_,Order_++)),
        //Can't iterate under this condition (it occurs quite naturally in
        //recursion)
        Done_(MaxOrder_<MinOrder_){}

template<typename T>
void PowerSetItr<T>::Next(){
   ++(*CurrentIt_);
   if(!CurrentIt_->Done())return;
   else if(Order_<=MaxOrder_)
     CurrentIt_=SharedItr_t(new CombItr_t(Set_,Order_++));
   else Done_=true;
}


}}//End namespace


#endif /* POWERSETITR_HPP */

