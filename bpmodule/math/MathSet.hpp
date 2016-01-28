/* 
 * File:   MathSet.hpp
 * Original Author: Ryan Richard <ryanmrichard1@gmail.com>
 *
 * Created on January 22, 2016, 11:11 AM
 */

#ifndef BPMODULE_GUARD_MATHSET_HPP_
#define BPMODULE_GUARD_MATHSET_HPP_

#include <vector> //For default set container
#include <algorithm> //For set operations
#include "bpmodule/exception/Assert.hpp"
#include "bpmodule/exception/Exceptions.hpp"
#include "bpmodule/math/Universe.hpp"

namespace bpmodule{
namespace math{
   
/** \brief A class for performing set manipulations efficiently relative to
 *   some universe.
 * 
 *   With the universe class all of the operations are deep copies or involve
 *   copying the actual elements over.  Now that we know what our universe is,
 *   we can do these operations much more efficiently by mapping each element
 *   to an integer in the range 0 to the size of our universe and performing
 *   the operations on those integers.  That's what this class does.  Thus
 *   by default operations with this class are shallow copies.
 *   
 *   Because a MathSet derives from Universe it is perfectly capable of being
 *   itself used as a universe.  Note that the privateness of the Universe
 *   base class means you can't pass this class by it's base class to anything
 *   other than this class.
 *   
 */
template<typename T,typename U=std::vector<T>>
class MathSet:private Universe<T,U>{
private:
    ///Base class's type
    typedef Universe<T,U> Base_t;
    ///This class's type
    typedef MathSet<T,U> My_t;
    
    ///The universe of discourse for the set
    std::shared_ptr<const Base_t> Universe_;
    
    ///Checks if Elem is in the universe
    void InUniverse(const T& Elem)const{
        exception::Assert<exception::ValueOutOfRange>(
        Universe_->Contains(Elem),
        "Requested element is not in the universe for this set"
        );
    }
    
    ///Normal copy of our class (code factorization)
    void Copy(const My_t& RHS){
        this->Elems_=RHS.Elems_;
        this->Storage_=RHS.Storage_;
        Universe_=RHS.Universe_;
    }
    
public:
    
     MathSet()=default;
    
    ///Makes a set that is part of the given universe
    MathSet(std::shared_ptr<const Base_t> AUniverse):
        Universe_(AUniverse){}
    
    ///Deep copies elements, shallow copies Universe_ and Storage_
    MathSet(const My_t& RHS):Base_t(){Copy(RHS);}
    
    ///Returns a deep copy of everything
    virtual My_t Clone()const{
        My_t Temp;
        //Deep copies this's member data
        Temp.Base_t::operator=(*this);
        //Deep copies the universe
        Temp.Universe_=std::shared_ptr<Base_t>(new Base_t(*Universe_));
        return Temp;
    }
    
    ///Same as copy constructor, but for assignment
    My_t& operator=(const My_t& RHS){
        if(this!=&RHS)Copy(RHS);
        return *this;
    }
    
    ///Unlike the base class, we just add the index
    virtual My_t& operator<<(const T& Elem){
        InUniverse(Elem);
        this->Elems_.insert(Idx(Elem));
        return *this;
    }
    
    ///Makes this the union of this and other
    const My_t& operator+=(const My_t& RHS){
        for(const size_t& EI : RHS.Elems_)
            this->Elems_.insert(EI);
        return *this;
    }
    
    ///Returns the union of this and other (result shares same resources)
    My_t operator+(const My_t& RHS)const{
        return My_t(*this)+=RHS;
    }
    
    ///Makes this the intersection of this and other
    const My_t& operator/=(const My_t& RHS){
        std::set<size_t> Temp(std::move(this->Elems_));
        this->Elems_.clear();
        std::set_intersection(Temp.begin(),Temp.end(),
                              RHS.Elems_.begin(),RHS.Elems_.end(),
                              this->Elems_.begin());
        return *this;
    }
    
    ///Returns the intersection of this and other
    My_t operator/(const My_t& RHS)const{
        return My_t(*this)/=RHS;
    }
    
    ///Makes this the set-difference of this and other
    const My_t operator-=(const My_t& RHS){
        std::set<size_t> Temp(std::move(this->Elems_));
        this->Elems_.clear();
        std::set_difference(Temp.begin(),Temp.end(),
                            RHS.Elems_.begin(),RHS.Elems_.end(),
                            this->Elems_.begin());
        return *this;
    }
    
    ///Returns the set-difference of this and other
    My_t operator-(const My_t& RHS)const{
        return My_t(*this)-=RHS;
    }
    
    ///Returns the complement of this and other
    My_t Complement()const{
        My_t Temp;
        Temp.Storage_=this->Storage_;
        Temp.Universe_=Universe_;
        std::set_difference(Universe_->Elems_.begin(),Universe_->Elems_.end(),
                            Elems_.begin(),Elems_.end(),Temp.Elems_.begin());
        return Temp;
    }
    
    ///@{
    ///Basic forwarding of remaining functions and types
    
    virtual std::string ToString()const{
        return Base_t::ToString();
    }
    
    typedef typename Base_t::iterator iterator;
    typedef typename Base_t::const_iterator const_iterator;
    iterator begin(){return Base_t::begin();}
    const_iterator begin()const{return Base_t::begin();}
    iterator end(){return Base_t::end();}
    const_iterator end()const{return Base_t::end();}
    ///@}
 };   


}
}//End namespaces

#endif /* BPMODULE_GUARD_MATHSET_HPP_ */

