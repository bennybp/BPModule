/* 
 * File:   MathSet.hpp
 * Original Author: Ryan Richard <ryanmrichard1@gmail.com>
 *
 * Created on January 22, 2016, 11:11 AM
 */

#ifndef BPMODULE_GUARD_MATHSET_HPP_
#define BPMODULE_GUARD_MATHSET_HPP_

#include <set>
#include <algorithm>
#include "bpmodule/exception/Assert.hpp"
#include "bpmodule/exception/Exceptions.hpp"

namespace bpmodule{
namespace math{

/** \brief A class that implements a mathematical set
 * 
 *   We have added a few features on top of the std::set.
 *   Namely, we can now do unions, intersections, set diff, etc.
 *   as well as complements because we insist on having each set know
 *   its universe of discourse.  The class is designed to allow for working with
 *   very large sets in an optimized and object-oriented manner.  In particular
 *   the MathSet allows the actual element data to be stored in a separate 
 *   object, which all sets will contain a shared pointer to.  We term this 
 *   object the storage container.  In particular we use this feature to be
 *   able to store our points contigiously, while being able to store each
 *   point as a separate object (whose real data lives in the storage container
 *   ).
 * 
 *   We have to distinguish between two scenarios: working with a universe and
 *   working with sets within an existing universe.  We will term the first
 *   scenario universe creation mode or UCM.  In UCM, we allocate the memory in
 *   the storage container as well as define which elements can show up in
 *   the universe.  UCM is triggered by using the default constructor or by
 *   working with a MathSet that is a universe (can be checked by calling
 *   IsUniverse).
 * 
 *   Once you have a universe you can start making sets that live within that
 *   universe (note the universe itself is a perfectly usable set, so if you
 *   want all elements there's no need to make a new set).  To do this you use
 *   the constructor that takes a shared_ptr.  The shared_ptr must point to a
 *   valid instance of a MathSet or else you will enter UCM.  Now when you
 *   add elements only the elements of type T will be copied, not the data
 *   inside the storage container.
 * 
 *   Note that subsets can be used as universes themselves; however, one can not
 *   modify the elements they contain.  We distinguish between modifiable and
 *   non-modifiable universes by termming the former: top-level universes.  If 
 *   one wants to use subsets of different universes to create a new universe,
 *   the elements must be deep copied and inserted into a default constructed
 *   MathSet, thus upgrading them to top-level universes.  Interestingly, this
 *   is completely consistent with the mathematical notion that one can only
 *   combine sets that belong to the same universe by realizing that our 
 *   top-level universes are subsets of the true universe (the set of all 
 *   possible elements), which in general is the only universe in which
 *   arbitrary combining operations are possible.
 *    
 *   Note for PODs no special memory semantics are needed so the universe will
 *   contain two copies of each element, one in the storage container and one in
 *   the Elems_ array.  Its subsets, however, will only contain elements in
 *   the Elems_ array, and only those actually in the set.
 * 
 *   \param T The type of the element.  Should be very light weight, will be
 *            copied a lot.  Must be default constructable and comparable with
 *            the less operator.
 *   \param U The type of the storage class.  This is a class that will hold the
 *            memory for the elements.  Must be default constructable, capable
 *            of returning an iterator to the end of the container via `end()`
 *            and support insertion via a function with signature:
 *            `X insert(end(),const T& value)` where X is an object that is
 *            dereferencable to an object of type T
 */
template<typename T,typename U=std::set<T> >
class MathSet{
private:
    ///The universe of discourse for the set.  If NULL this is a top-level 
    ///universe that belongs to the true universe.
    std::shared_ptr<const MathSet<T> > Universe_;

    ///The elements in this set
    std::set<T> Elems_;
    
    ///The storage class
    std::shared_ptr<U> Storage_;
    
    ///Checks if element is in Universe_, throws if it is not, basically code
    ///factorization
    bool InUniverse(const T& elem)const;
    
public:
    ///An iterator to the elements in this set
    typedef typename std::set<T>::iterator iterator;
    ///An iterator to const versions of the elements in this set
    typedef typename std::set<T>::const_iterator const_iterator;

    ///Makes a set that is part of the given universe (or a set that is a top-
    ///level universe if NULL)
    MathSet(std::shared_ptr<MathSet<T> > AUniverse=
            std::shared_ptr<MathSet<T> >()):
        Universe_(AUniverse){
    }
    
    ///Performs a shallow copy
    MathSet(const MathSet<T,U>& /*Other*/)=default;

    
    ///No special memory clean-up
    ~MathSet()=default;

    iterator begin(){return Elems_.begin();}
    const_iterator begin()const{return Elems_.begin();}
    iterator end(){return Elems_.end();}
    const_iterator end()const{return Elems_.end();}
    
    
    ///True if this is a top-level universe
    bool IsUniverse()const{return !Universe_;}
    
    /** \brief Adds an element to this set, returns this
     * 
     * Lots of magic happens here.  If we are a top-level universe
     * we have to allocate the memory in the storage container for the new
     * element.  We do this by calling insert on the storage container and
     * assuming it has a signature:
     * \code
     * iterator insert(iterator,T);
     * \endcode
     * The resulting iterator should point to the newly added element and this
     * is what is added to the MathSet.  This signature and behavior is
     * satisfied by all STL containers, as well as our PointStorage class.
     * 
     * If we a subset, then the only thing that happens is the
     * element is added to this set.  In a debug build, this function checks to
     * see if the element is in the universe before adding it.
     * 
     * Either way, to the user, this looks like it does the same thing
     * 
     * Note: calling this function invalidates all iterators that are
     * out
     * 
     */
    MathSet<T,U>& operator<<(const T& Elem);

    ///Returns true if this set contains Elem
    bool Contains(const T& Elem)const{
        return Elems_.count(Elem)==1;
    }

    ///Returns the i-th element
    T operator[](size_t i)const{
        exception::Assert<exception::ValueOutOfRange>(
          i<Elems_.size(),
          "MathSet contains less elements than requested element"
        );
        return *(std::next(Elems_.begin(),i));
    }
    
    
    ///Makes this, the union of this and other, returns this
    const MathSet<T,U>& operator+=(const MathSet<T,U>& RHS);
    
    ///Returns the union of this and other
    MathSet<T,U> operator+(const MathSet<T,U>& RHS)const{
        return (MathSet<T,U>(*this))+=RHS;
    }
    
    ///Makes this the intersection of this and other, returns this
    const MathSet<T,U>& operator/=(const MathSet<T,U>& RHS);
    
    ///Returns the intersection of this and other
    MathSet<T,U> operator/(const MathSet<T,U>& RHS)const{
        return (MathSet<T,U>(*this))/=RHS;
    }
    
    ///Makes this the set-difference of this and other, returns this
    const MathSet<T,U>& operator-=(const MathSet<T,U>& RHS);
    
    ///Returns the set-difference of this and other
    MathSet<T,U> operator-(const MathSet<T,U>& RHS)const{
        return (MathSet<T,U>(*this))-=RHS;
    }
    
    ///Returns the complement of this
    MathSet<T,U> Complement()const{
        exception::Assert<exception::ValueOutOfRange>(
            Universe_,
            "Complement must be taken in a universe"
        );
        MathSet<T,U> temp(*Universe_);
        temp.Universe_=Universe_;
        temp-=*this;
        return temp;
    }
};

template<typename T,typename U>
bool MathSet<T,U>::InUniverse(const T& Elem)const{
    if(!IsUniverse())
        exception::Assert<exception::ValueOutOfRange>(
            Universe_->Contains(Elem),
            "Element is not in universe"
        );
    return true;
}

template<typename T,typename U>
MathSet<T,U>& MathSet<T,U>::operator<<(const T& Elem){
    if(IsUniverse()){
        //Scenario: we are the universe and thus responsible for memory
        if(!Storage_)Storage_=std::shared_ptr<U>(new U);
        Elems_.insert(Elems_.begin(),*(Storage_->insert(Storage_->end(),Elem)));
    }
    else{
        InUniverse(Elem);
        Elems_.insert(Elem);
    }
    return *this;
}

template<typename T,typename U>
const MathSet<T,U>& MathSet<T,U>::operator+=(const MathSet<T,U>& RHS){
    const_iterator EI=RHS.begin(),EIEnd=RHS.end();
    iterator Hint=Elems_.begin();
    exception::Assert<exception::ValueOutOfRange>(
            Universe_.get()!=RHS.Universe_.get(),
            "Universes are not equal in union"
            );
    for(;EI!=EIEnd;++EI){
        if(!IsUniverse())//Check that element is in universe
            exception::Assert<exception::ValueOutOfRange>(
                Universe_->Contains(*EI),
                "Element is not in universe"
                );
        Hint=Elems_.insert(*EI,Hint);
    }
    return *this;
}

template<typename T,typename U>
const MathSet<T,U>& MathSet<T,U>::operator/=(const MathSet<T,U>& RHS){
    exception::Assert<exception::ValueOutOfRange>(
        Universe_.get()!=RHS.Universe_.get(),
        "Universes are not equal in intersection"
        );
    std::set<T> LHS=Elems_;
    Elems_.clear();
    std::set_intersection(LHS.begin(),LHS.end(),RHS.begin(),RHS.end(),
            std::inserter<std::set<T>>(Elems_,Elems_.begin()));
    return *this;
}

template<typename T,typename U>
const MathSet<T,U>& MathSet<T,U>::operator-=(const MathSet<T,U>& RHS){
    exception::Assert<exception::ValueOutOfRange>(
        Universe_.get()!=RHS.Universe_.get(),
        "Universes are not equal in intersection"
        );
    std::set<T> LHS=Elems_;
    Elems_.clear();
    std::set_difference(LHS.begin(),LHS.end(),RHS.begin(),RHS.end(),
            std::inserter<std::set<T>>(Elems_,Elems_.begin()));
    return *this;
}


}
}//End namespaces

#endif /* BPMODULE_GUARD_MATHSET_HPP_ */

