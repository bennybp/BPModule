/* 
 * File:   PowerSetItr.hpp
 *
 * Created on March 17, 2016, 6:07 PM
 */

#pragma once

#include <memory>
#include "pulsar/math/CombItr.hpp"
namespace pulsar{

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
 *  Note for posterity, I had coded this iterator up in terms of counting in
 *  binary, which is based on the realization that the binary
 *  representation of \f$2^N-1\f$ contains \f$N\f$ 1's.  So counting
 *  from 0 to \f$2^N-1\f$, in binary will generate all of the
 *  \f$2^N\f$ subsets of \f$S\f$ if we associate a 1 in the \f$i\f$-th
 *  place with \f$S_i\f$ being in the current subset and a 0 with its
 *  absence.  Unfortunately this generates the subsets in a weird
 *  order, something akin to (assuming \f$S=\{1,2,3,\cdots,N\}\f$):
 *  \verbatim
   (empty set)
   1
   2
   1 2
   3
   1 3
   2 3
   1 2 3
   ...
   1 2 3...N
   \endverbatim
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
 *  times.  I suspect this is not as effecient as the counting in binary, but
 *  I haven't timed it.
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
    for(;!MyItr.done();++MyItr){
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
 *
 * \param T The type of the set we are iterating over, will also be
 *          the type of susbsets returned by the iterator.
 * 
 * \note This class is not exported to Python and itertools does not provide a
 *     direct equivalent, but one can easily use chain() 
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
      typedef typename pulsar::CombItr<T> CombItr_t;
      typedef typename std::shared_ptr<CombItr_t> SharedItr_t;
      SharedItr_t CurrentIt_;
      ///Have we iterated over the entire range yet?
      bool Done_;
      ///Implementation for getting the next element
      void next();
   public:
      /** \brief Given a set, iterates over all subsets containing Min
       *      number of elements to Max number of elements
       *
       *  Often one only wants to iterate over part of the power set, which
       *  is what this function does.  Specifically it iterates from sets
       *  containing \p Min elements to those containing \p Max elements
       *  inclusively.  Note that this is not usual C++ counting (Min=1 really  
       *  gives you sets with 1 element and not 2).
       */
      PowerSetItr(const T& Set, size_t Min, size_t Max);
      ///Iterates over the entire power set
      PowerSetItr(const T& Set):PowerSetItr(Set,0,Set.size()){}
      ///Deep copies other iterator
      PowerSetItr(const PowerSetItr&);
      ///Returns true if we have iterated over the whole range
      bool done()const{return Done_;}
      ///Returns true if there are combinations left
      operator bool()const{return !Done_;}
      ///Moves on to the next subset and returns it
      PowerSetItr<T>& operator++(){next();return *this;}
      ///Moves to next, returns current
      PowerSetItr<T> operator++(int);
      ///Returns the current subset
      const T& operator*()const{return CurrentIt_->operator*();}
      ///Allows access of the subset's container's members
      const T* operator->()const{return CurrentIt_->operator->();}
};

/******************** Implementations ******************/
template<typename T>
PowerSetItr<T>::PowerSetItr(const PowerSetItr<T>& other)
   : Set_(other.Set_),MaxOrder_(other.MaxOrder_),MinOrder_(other.MinOrder_),
        Order_(other.Order_),
        CurrentIt_(other.CurrentIt_?std::make_shared<CombItr_t>(*other.CurrentIt_):nullptr),
        Done_(other.Done_)
{
          
}


template<typename T>
PowerSetItr<T> PowerSetItr<T>::operator++(int)
{
    PowerSetItr<T> temp(*this);
    next();
    return temp;
}

template<typename T>
PowerSetItr<T>::PowerSetItr(const T& Set, size_t Min,size_t Max):
        Set_(Set),
        MaxOrder_(Max),
        MinOrder_(Min),
        Order_(MinOrder_),
        CurrentIt_(std::make_shared<CombItr_t>(Set_,Order_++)),
        //Can't iterate under this condition (it occurs quite naturally in
        //recursion)
        Done_(MaxOrder_<MinOrder_||Max==0){}

template<typename T>
void PowerSetItr<T>::next(){
   ++(*CurrentIt_);
   if(!CurrentIt_->done())return;
   else if(Order_<=MaxOrder_)
     CurrentIt_=std::make_shared<CombItr_t>(Set_,Order_++);
   else Done_=true;
}


}//End namespace

