/* 
 * File:   MathSet.hpp
 * Original Author: Ryan Richard <ryanmrichard1@gmail.com>
 *
 * Created on January 22, 2016, 11:11 AM
 */

#ifndef BPMODULE_GUARD_MATHSET_HPP_
#define BPMODULE_GUARD_MATHSET_HPP_

#include <set>
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
 *   object the storage container.
 * 
 *   We have to distinguish between two scenarios: working with a universe and
 *   working with sets of an existing universe.  We will term the first universe
 *   creation mode or UCM.  In UCM, we allocate the memory in the storage
 *   container.  UCM is triggered by using the default constructor.
 * 
 * 
 *   The example below will likely explain most of this:
 *   \code
 *      //Default constructor gives us UCM
 *      MathSet<char> U;
 *      
 *      //Adds three elements to U and allocates their memory
 *      U<<'a'<<'b'<<'c';
 * 
 *      //Make a set that will be a subset of U
 *      MathSet<char> S(U);
 *      
 *      //Add the elements a and b to S
 *      S<<'a'<<'b';
 * 
 *      //Compute the complement of S
 *      MathSet<char> SC=S.Complement();
 * 
 *      //SC is now 'c'
 *   \endcode
 *   To the user the difference between normal operation and UCM are pretty
 *   transparent other than
 *    
 *      
 * 
 *   \param T The type of the element.  Should be very light weight, will be
 *            copied a lot.
 *   \param U The type of the storage class.  This is a class that will hold the
 *            memory for the elements.
 */
template<typename T,typename U=std::set<T> >
class MathSet{
private:
    ///The universe of discourse for the set.  If NULL this is a top universe
    std::shared_ptr<const MathSet<T> > Universe_;

    ///The elements in this set
    std::set<T> Elems_;
    
    ///The storage class
    std::shared_ptr<U> Storage_;
    
public:
    ///An iterator to the elements in this set
    typedef typename std::set<T>::iterator iterator;
    ///An iterator to const versions of the elements in this set
    typedef typename std::set<T>::const_iterator const_iterator;
    
    ///Makes an empty set, resulting set can be used as a universe
    MathSet()=default;

    ///Makes a set that is part of the given universe
    MathSet(std::shared_ptr<MathSet<T> > AUniverse):
        Universe_(AUniverse){
    }
    
    virtual ~MathSet()=default;

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
     * is what is added to the MathSet.
     * 
     * If we belong to a universe, then the only thing that happens is the
     * element is added to this set.  In a debug build, this function checks to
     * see if the element is in the universe before adding it.
     * 
     * Either way, to the user, this looks like it does the same thing
     * 
     */
    MathSet<T,U>& operator<<(const T& Elem);

    ///Returns true if this set contains Elem
    bool Contains(const T& Elem)const{
        return Elems_.count(Elem)==1;
    }

    ///Makes this, the union of this and other, returns this
    const MathSet<T,U>& operator+=(const MathSet<T,U>& LHS);
    
    ///Returns the union of this and other
    MathSet<T,U>&& operator+(const MathSet<T,U>& LHS)const{
        MathSet<T,U> Return(Universe_);
    }
};

template<typename T,typename U>
MathSet<T,U>& MathSet<T,U>::operator<<(const T& Elem){
    if(Universe_){
        exception::Assert<exception::ValueOutOfRange>(
            Universe_->Contains(Elem),
            "Element is not in universe"
        );
        Elems_.insert(Elem);
    }
    else{//Scenario where we are the universe and thus responsible for memory
        if(!Storage_)
            Storage_=std::shared_ptr<U>(new U);
        Elems_.insert(*(Storage_->insert(Storage_->back(),Elem));
    }
    return *this;
}

template<typename T,typename U>
const MathSet<T,U>& MathSet<T,U>::operator+=(const MathSet<T,U>& LHS){
    const_iterator EI=LHS.begin(),EIEnd=LHS.end();
    iterator Hint=Elems_.begin();
    for(;EI!=EIEnd;++EI){
        exception::Assert<exception::ValueOutOfRange>(
                Universe_->Contains(*EI),
                "Element is not in universe"
                );
        Hint=Elems_.insert(*EI,Hint);
    }
    return *this;
}

}
}//End namespaces

#endif /* BPMODULE_GUARD_MATHSET_HPP_ */

