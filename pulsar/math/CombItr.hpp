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
namespace system{
class System;
}
namespace math{

/** \brief An iterator to generate all combinations from n objects,
 *         k at a time, i.e. all k-combinations
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
 *  arbitrary sets) of the positions in your input set, not their values 
 *  i.e. if you  gave the set {2,3,1} and wanted all pairs you would get
 *  (positions shown in parentheses):
 *  \verbatim
 *  2 3 (0 1)
 *  2 1 (0 2)
 *  3 1 (1 2)
 *  \endverbatim
 *  Note that this is distinct from:
 *  \verbatim
 *  1 2
 *  1 3
 *  2 3
 *  \endverbatim
 *  which is lexical order of the elements in that set.
 *
 *  Here's all you need to use this class.  Assume you have a container
 *  of type MyContainerType, then:
 *  \code
 *  //The type of the combination iterator
 *  typedef CombItr<MyContainerType> ItrType;
 *
 *  //Get your already filled container somehow
 *  MyContainerType MySet=MagicalFunction();
 * 
 *  //Assuming we want all k-combinations
 *  ItrType(MySet,K);
 *
 *  //Now loop over combinations
 *  while(MyItr){
 *     MyContainerType CurrentComb=*MyItr;
 *
 *     //Do stuff with combination
 * 
 *     //Increment iterator
 *     ++MyItr;
 *  }
 *  \endcode
 *  
 *  Note that you get your combinations back in whatever container type we
 *  are iterating over.

    \param T The type of the set you want the combinations of.
 *           Must minimally be:
 *           - Default constructable
 *           - Forward iterable (have begin() and end() functions)
 *           - A member type called const_iterator which is a typedef of a 
 *             constant iterator to that class.
 *           - Possess a function insert(const_iterator,U) that inserts an
 *             element of type U (whatever type is your set) BEFORE the
 *             iterator passed in
 *           Basically all containers in the STL satisfy these criteria (I
 *           think only queue, stack, and array do not satisfy them.  Queue
 *           and stack basically fail all criteria, array does not have an
 *           insert member)
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
      ///The set the user gave us
      const T& Set_;
      
      ///Are we done?
      bool Done_;
      
      ///Constructor code factorization
      void Initialize();
      
      ///Sets Comb_ to the next combination
      void Next();
      
      ///Function that resets Comb_
      void ResetComb();
   public:
      ///Makes a new iterator that will run through Set, K at a time
      CombItr(const T& Set, size_t K);
      ///Makes a copy of the other iterator
      CombItr(const CombItr<T>& Other)=default;
      ///Returns the combination this iterator points to
      const T& operator*()const{return Comb_;}
      ///Returns true if we have run through all combinations
      bool Done()const{return Done_;}
      ///True while the iterator still has combinations left
      operator bool()const{return !Done();}
      ///Allows access to the member functions of your container
      const T* operator->()const{return &Comb_;}
      ///Increments the iterator before returning it
      CombItr<T>& operator++(){Next();return *this;}
      ///Increments the iterator after returning it
      CombItr<T> operator++(int){CombItr<T> Temp(*this);Next();return Temp;}
      
};

///Specializations of CombItr for system
template<> void math::CombItr<system::System>::ResetComb();
template<> math::CombItr<system::System>::CombItr(const system::System& Set,size_t K);

/*********** Implementations ***************/
template <typename T>
CombItr<T>::CombItr(const T& Set, size_t K) :
      K_(K),Indices_(K),Set_(Set), Done_(false) {
    Initialize();
}

template<typename T>
void CombItr<T>::Initialize(){
    if (Set_.size()<K_)
    throw pulsar::exception::GeneralException(
           "I don't know how to generate combinations with"
           " more items than you gave me....",
           "NObjects",Set_.size(),"Requested K",K_);

   //Form the starting combination
   TItr_t ElemI=Set_.begin();
   for(size_t i=0;i<K_;++i,++ElemI){
       Indices_[i]=ElemI;
       Comb_.insert(Comb_.end(), *ElemI);       
   }
}

template <typename T>
void CombItr<T>::Next() {
    //Check indices right to left
    for(size_t i=1;i<=K_;++i){
        const size_t Offset=K_-i;
        TItr_t NewIdx=Indices_[Offset];
        ++NewIdx;
        bool Good=false;
        //Try to reset all indices to the right
        for(size_t j=0;j<i;++j,++NewIdx){
            if(NewIdx==Set_.end())break;//i can't be incremented anymore
            Indices_[Offset+j]=NewIdx;
            if(j==i-1)Good=true;//Successfully incremented all indices
        }
        if(Good)break;//Combination is good we are done
        else if(i==K_){//Out of combinations
            Done_=true;
            return;
        }
    }
    ResetComb();
    for(const TItr_t Idx: Indices_)
        Comb_.insert(Comb_.end(),*Idx);
}

template<typename T>
void CombItr<T>::ResetComb() {
    //Fill in combination
    T NewComb;
    Comb_=NewComb;    
}

}}//End namespaces
#endif /* COMBITR_HPP */

