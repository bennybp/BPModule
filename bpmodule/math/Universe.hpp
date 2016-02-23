#ifndef BPMODULE_GUARD_MATH__UNIVERSE_HPP_
#define BPMODULE_GUARD_MATH__UNIVERSE_HPP_
/* 
 * \file   Universe.hpp
 * Original Author: richard
 * 
 * Created on January 27, 2016, 6:00 PM
 * 
 * \brief Definitions of a SetItr and a set Universe
 */

#include <set> //Where the indices live
#include <memory> //Shared ptrs
#include <algorithm> //For std::find
#include <sstream> //For printing

namespace bpmodule {
namespace math{
    
template<typename T,typename U>
class Universe;
    
/** An iterator to go with the universe class, returns actual objects
 *
 * The iterator only allows accessing the data, not modifying. That is,
 * this only behaves like a const_iterator
 */
template<typename T,typename U>
class ConstSetItr{
    private:
        ///My type
        typedef ConstSetItr<T,U> My_t;
        ///Type of the iterator to the Indices in the universe
        typedef std::set<size_t>::const_iterator Itr_t;
        ///The index I currently point to
        Itr_t CurrIdx_;
        ///The universe I am tied to
        const U* Set_;
        ///Let universe play with my private parts
        friend Universe<T,U>;
        ///Only universe can make a working iterator
        ConstSetItr(Itr_t CurrIdx,const U& Set):
            CurrIdx_(CurrIdx),Set_(&Set){}
    public:
        ///Returns true if this iterator is equal to RHS
        bool operator==(const My_t& RHS)const{
            return (CurrIdx_==RHS.CurrIdx_);
        }
        ///Returns true if this iterator is not equal to RHS
        bool operator!=(const My_t& RHS)const{
            return !this->operator ==(RHS);
        }
        ///Returns a reference to the current element
        const T & operator*()const{
            return (*Set_)[*CurrIdx_];
        }

        ///Prefix increment operator
        const My_t& operator++(){
            ++CurrIdx_;
            return *this;
        }
        ///Postfix increment operator
        const My_t& operator++(int){
            My_t ret(*this);
            ++CurrIdx_;
            return ret;
        }
        
};


/** \brief A class that implements a mathematical (ordered) set that is a
 *  universe.  Order is defined as that in which the elements are inserted.
 * 
 *   Note every mathematical set is capable of being viewed as a universe,
 *   which is why MathSet derives from this class.  MathSet differs from
 *   Universe in that MathSet contains a pointer to the Universe it belongs
 *   to, whereas Universe's universe is all possible elements.
 * 
 *   Essentially, We have added a few features on top of the std::set.
 *   Namely, we can now do unions, intersections, set diff, etc.
 *   as well as complements.  To do the latter, we need a universe which is what
 *   this class is. The class is designed to allow for working with
 *   very large sets in an optimized and object-oriented manner.  In particular
 *   the Universe allows the actual element data to be stored in a separate 
 *   object, which all sets will contain a shared pointer to.  Subsets, which
 *   are objects of type MathSet, only store the indices of the elements in the
 *   storage container that they contain.  As long as the storage class defines
 *   an interface, which we layout below, any class can be used to fill this
 *   role, in particular we use this feature to be
 *   able to store our points contigiously, while being able to store each
 *   point as a separate object (whose real data lives in the storage container
 *   ).
 * 
 *   It is notable that copy,assignment, and binary operations (i.e. not +=, -=,
 *   etc.) return deep copies of the universe.  
 * 
 *   \param T The type of the element.  Should be very light weight, will be
 *            copied a lot.  Must be default constructable and comparable with
 *            the less operator.
 *   \param U The type of the storage class.  This is a class that will hold the
 *            actual memory for the elements.  Must minimally have an interface:
 *            \code
 *            class SetStorage{
 *            public:
 *               //Fucntion that returns an iterator to the first element
 *               iterator begin();
 *               //Function that returns an iterator just past the last element
 *               iterator end();
 *               //Function that inserts an element, Elem, 
 *               //given end()'s iterator
 *               iterator insert(iterator,const T& Elem);
 *               //returns a copy of Elem i
 *               T operator[](size_t i)const;
 *            }
 *            \endcode
 *            Note that std::vector satisfies these requests and is the default
 */    
template<typename T,typename U=std::vector<T>>
class Universe{
private:
    ///My type
    typedef Universe<T,U> My_t;
    ///The storage class
    std::shared_ptr<U> Storage_;
protected:
    ///The elements in this set
    std::set<size_t> Elems_;
public:
    typedef T value_type;

    ///An iterator to const versions of the elements in this set
    typedef ConstSetItr<T,U> const_iterator;
    
    ///Deep copies the universe
    Universe(const My_t& RHS) : Storage_(new U(*RHS.Storage_)), Elems_(RHS.Elems_){
    }
        
    
    ///Deep copies during assignment
    const My_t& operator=(My_t RHS){
        using std::swap;
        if(this!=&RHS)swap(*this, RHS);
        return *this;
    }
    
    ///Makes a empty universe
    Universe() : Storage_(new U) { };
    
    ///No special memory clean-up
    virtual ~Universe()=default;
    
    ///@{
    ///Basic accessors
    ///Returns the cardinality of the universe (i.e. the number of elements)
    size_t size()const noexcept{return Elems_.size();}
    ///Returns an iterator to a const version of the 
    virtual const_iterator begin()const{
        return const_iterator(Elems_.begin(),*Storage_);
    }
    ///Returns an iterator just past a const version of the last element
    virtual const_iterator end()const{
        return const_iterator(Elems_.end(),*Storage_);
    }
    ///Returns the index of an Elem in Storage_
    virtual size_t Idx(const T& Elem)const{
        return std::distance(
                Storage_->begin(),
                std::find(Storage_->begin(),Storage_->end(),Elem)
        );
    }
    T& operator[](size_t EI){return (*Storage_)[EI];}
    const T& operator[](size_t EI)const{return (*Storage_)[EI];}
    ///@}

    /** \brief Adds an element to this set, returns this
     * 
     * This function adds an element to the universe and allocates
     * memory for it in the storage class.  If the storage class
     * is not allocated this function allocates it.  Ultimately, all
     * insertion calls go through this function.
     * 
     * Note: calling this function invalidates all iterators that are
     * out
     */
    virtual My_t& operator<<(const T& Elem){
        if(!Storage_)Storage_=std::shared_ptr<U>(new U);
        Elems_.insert(Storage_->size());
        Storage_->insert(Storage_->end(),Elem);
        return *this;
    }

    ///Returns true if this set contains Elem, comparison occurs via
    ///Elem's operator==
    bool Contains(const T& Elem)const{
        return Contains(Idx(Elem));
    }
    bool Contains(size_t EI)const{
        return Elems_.count(EI)==1;
    }
    
    /** \brief Makes this the union of this and RHS
     * 
     *  We do not want to sort the data in Storage_, so we can not use 
     *  std::set_union.  We implement this operation by iterating over
     *  all elements in RHS, checking if they are in this, and if not,
     *  adding them.  New elements are appended to the end of this's storage,
     *  in the order they appear in RHS.  If this isn't good enough you likely
     *  will have to merge the sets manually.
     */
    const My_t& operator+=(const My_t& RHS){
        for(const T & EI : RHS)
            if(!this->Contains(EI))(*this)<<EI;
        return *this;
    }
    
    ///Returns a new universe (not linked to this one) that is union of this and
    ///RHS
    My_t operator+(const My_t& RHS)const{
        return My_t(*this)+=RHS;
    }
    
    /** \brief Makes this the intersection of this and RHS
     * 
     *  Again, we don't want to sort and so we can't use std::set_intersection.
     *  The elements contained in this after the operation, will be in the same
     *  order they were  originally.
     */
    const My_t& operator/=(const My_t& RHS){
        std::shared_ptr<U> Temp(new U);
        std::set<size_t> TempElems;
        for(const T & Elem : *this)
            if(RHS.Contains(Elem)){
                TempElems.insert(Temp->size());
                Temp->insert(Temp->end(),Elem);
            }
        Storage_=std::move(Temp);
        Elems_=std::move(TempElems);
        return *this;
    }
    
    ///Returns a new universe, (not linked to this one) that is the intersection
    My_t operator/(const My_t& RHS)const{
        return My_t(*this)/=RHS;
    }
    
    /** \brief Makes this the set difference of this and RHS
     * 
     *  Again, we can't sort Storage_, so we can't use std::set_difference. As
     *  with intersection, the final order is the same as it was orginally
     * 
     */
    const My_t& operator-=(const My_t& RHS){
        std::shared_ptr<U> Temp(new U);
        std::set<size_t> TempElems;
        for(const T & EI : *this)
            if(!RHS.Contains(EI)){
                TempElems.insert(Temp->size());
                Temp->insert(Temp->end(),EI);
            }
        Storage_=std::move(Temp);
        Elems_=std::move(TempElems);
        return *this;
    }
    
    ///Returns a new universe (not linked to this one) that is the set-differnce
    My_t operator-(const My_t& RHS)const{
        return My_t(*this)-=RHS;
    }
    
    ///Helpful printing function
    virtual std::string ToString()const{
        std::stringstream ss;
        for(const T & EI: *this)ss<<EI<<" ";
        return ss.str();
    }
};

template<typename T,typename U>
std::ostream& operator<<(std::ostream& os, const Universe<T,U>& AUniv){
    return os<<AUniv.ToString();
}
    
}
}//End namespaces
#endif /* BPMODULE_GUARD_Universe_HPP_ */

