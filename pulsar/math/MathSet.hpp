/*
 * File:   MathSet.hpp
 * Original Author: Ryan Richard <ryanmrichard1@gmail.com>
 *
 * Created on January 22, 2016, 11:11 AM
 */

#pragma once

#include "pulsar/exception/Assert.hpp"
#include "pulsar/math/Universe.hpp"
#include "pulsar/util/IterTools.hpp"
#include <iterator>
namespace pulsar{

template<typename T,typename U> class MathSet;

/** An iterator to go with the MathSet class, returns actual objects
 *
 * The iterator only allows accessing the data, not modifying. That is,
 * this only behaves like a const_iterator
 */
template<typename T, typename U>
class ConstSetItr : public std::iterator<std::input_iterator_tag, const T> {
private:
    typedef ConstSetItr<T, U> My_t;///<Type of the iterator
    typedef Universe<T,U> Universe_t;///<Type of Universe in MathSet
    typedef std::set<size_t>::const_iterator Itr_t;///<Type of iterator to index
    Itr_t CurrIdx_;///<An iterator to the indices in the current MathSet
    const Universe_t* Set_;///<The Universe with the real data
    friend MathSet<T, U>;
    ///Only MathSet can make a working iterator
    ConstSetItr(Itr_t CurrIdx, const Universe_t* Set) :
        CurrIdx_(CurrIdx), Set_(Set) { }

public:
    typedef T value_type;///<The type this iterator returns

    ConstSetItr(const ConstSetItr&) = default;
    ConstSetItr& operator=(const ConstSetItr&) = default;
    ConstSetItr(ConstSetItr&&) = default;
    ConstSetItr& operator=(ConstSetItr&&) = default;
    ~ConstSetItr() = default;

    ///Returns true if this iterator is equal to RHS
    bool operator==(const My_t& RHS)const
    {
        return (CurrIdx_ == RHS.CurrIdx_ &&Set_ == RHS.Set_);
    }

    ///Returns true if this iterator is not equal to RHS
    bool operator!=(const My_t& RHS)const{return !this->operator==(RHS);}

    ///Returns current element
    const T & operator*()const{return (*Set_)[*CurrIdx_];}

    ///Allows access to current element member functions
    const T * operator->()const{return &((*Set_)[*CurrIdx_]);}

    ///Increments and then returns this
    My_t& operator++(){++CurrIdx_;return *this;}

    ///Returns a copy of this and then increments
    My_t operator++(int){My_t ret(*this);++CurrIdx_;return ret;}

};

/** \brief A class for performing set manipulations efficiently relative to
 *   some universe.
 *
 *   With the universe class all of the operations are deep copies or involve
 *   copying the actual elements over.  Now that we know what our universe is,
 *   we can do these operations much more efficiently by mapping each element
 *   to an integer in the range 0 to the size of our universe and performing
 *   the operations on those integers.  That's what this class does.  It is 
 *   entirely possible to simply use the Universe class for set manipulations 
 *   (barring complement), it will just not be as efficient.
 *
 *
 * \par Hashing
 *     The hash value of a MathSet is unique with respect to the values
 *     of the universe (see the Universe class documentation) and
 *     the values contained in this set. The hash will be
 *     the same even if the pointers point to different locations.
 *
 *   \param T The type of the object in the set
     \param U The type of the container holding the objects
 *
 */
template<typename T, typename U = std::vector<T>>
class MathSet {
private:
    typedef MathSet<T, U> My_t;///<This class's type
public:
    typedef ConstSetItr<T, U> const_iterator;///<An iterator to this class
    typedef T value_type;///<The type of the elements
    typedef U store_type;///<The type of the container
    typedef Universe<T, U> Universe_t;///<Type of the universe in this class
    ///Type of a function capable of selecting elements
    typedef std::function<bool(const T &) > SelectorFunc;
    ///Type of a function capable of transforming elements
    typedef std::function<T(const T &)> TransformerFunc;



    /// \name Constructors, destructors, assignment
    ///@{
    // constructs via shared pointer to universe and a given
    // set of elements
    MathSet(std::shared_ptr<const Universe_t> AUniverse,
            const std::set<size_t> & Elems)
    : Universe_(AUniverse)
    {
        this->Elems_ = Elems;
    }
    
    ///Makes a set in the universe and fills in all elements if fill==true
    explicit MathSet(std::shared_ptr<const Universe_t> AUniverse, bool fill)
          : Universe_(AUniverse)
    {
            for(size_t i : Range<0>(fill?AUniverse->size():0))
                Elems_.insert(i);
    }
    
    MathSet(const My_t&) = default;///<copies indices, aliases universe
    
    ///Default, but compiler bug prevents using = default
    MathSet(My_t&& RHS)
      :Universe_(std::move(RHS.Universe_)),Elems_(std::move(RHS.Elems_)){}
    
    ///Copies universe and optionally fills it
    MathSet(const My_t& RHS,bool fill):
        MathSet(RHS.Universe_,fill)
    {
    }
    
    
    
    /*! \brief For serialization only
     *
     * \warning NOT FOR USE OUTSIDE OF SERIALIZATION
     * \todo Replace if cereal fixes this
     */
    MathSet() {} // cannot be "=default" due to compiler bugs
                 // (symbol may be missing when compiled in Debug mode)



    ///Deep copies elements, shallow copies Universe_ and (and Storage_)
    My_t & operator=(const My_t & RHS){//appears to also not work defaulted
        if(this==&RHS)return *this;
        Universe_=RHS.Universe_;
        Elems_=RHS.Elems_;
        return *this;
    }
    
    MathSet & operator=(My_t&& RHS){//Getting undefined reference when defaulted
        Universe_=std::move(RHS.Universe_);
        Elems_=std::move(RHS.Elems_);
        return *this;
    }


    ///Returns a deep copy of everything
    My_t clone()const
    {
        return My_t(std::make_shared<Universe_t>(*Universe_), this->Elems_);
    }


    /// Obtain the universe used by this set
    std::shared_ptr<const Universe_t> get_universe(void)const{return Universe_;} 


    /*! \brief Obtain the contents of this MathSet as a new universe
     *
     * The new universe is not linked to this object in any way
     */
    Universe_t as_universe()const{return Universe_t().insert(begin(),end());}   

    ///@}

    ///@{
    ///Basic accessors
    
    ///Clears Elems does not delete universe
    void clear(void){Elems_.clear();}
    
    ///Returns the number of elements in this set
    size_t size(void)const noexcept{return Elems_.size();}

    ///Returns a constant iterator to the start of this set
    const_iterator begin() const
    {
        return const_iterator(Elems_.begin(),Universe_.get());
    }

    ///Returns a constant iterator just past the end of the this set
    const_iterator end() const
    {
        return const_iterator(Elems_.end(),Universe_.get());
    }

    ///Returns true if this set has the element
    bool count(const T& Elem)const
    {
        return (Universe_->count(Elem) && count_idx(idx(Elem)) > 0);
    }

    ///Returns true if the index is in the set
    bool count_idx(size_t Idx)const { return Elems_.count(Idx); }

    ///Returns the index of \p Elem, throws if \p Elem is not in the Universe
    size_t idx(const T& Elem)const{return Universe_->idx(Elem);}
    
    ///Inserts the element into the set, throws if \p Elem is not in Universe
    My_t& insert(const T & Elem)
    {
        universe_contains(Elem);
        Elems_.insert(idx(Elem));
        return *this;
    }
    
    ///Inserts the element with index \p Idx into this set
    My_t& insert_idx(size_t Idx)
    {
        universe_contains_idx(Idx);
        Elems_.insert(Idx);
        return *this;
    }
    ///@}


    /// \name Set operations
    ///@{

    ///Makes this the union of this and \p RHS
    My_t& union_assign(const My_t & RHS)
    {
        if(!SameUniverse(RHS))throw MathException("Incompatible universes");
        Elems_.insert(RHS.Elems_.begin(), RHS.Elems_.end());
        return *this;
    }

    ///Returns the union of this and \p RHS
    My_t set_union(const My_t& RHS)const
    {
        return My_t(*this).union_assign(RHS);
    }

    ///Makes this the intersection of this and \p RHS
    My_t& intersection_assign(const My_t& RHS);

    ///Returns the intersection of this and \p RHS
    My_t intersection(const My_t & RHS) const
    {
        return My_t(*this).intersection_assign(RHS);
    }

    ///Returns the difference
    My_t& difference_assign(const My_t& RHS);

    ///Returns the difference of this and \p RHS
    My_t difference(const My_t& RHS) const
    {
        return My_t(*this).difference_assign(RHS);
    }

    ///Returns the complement of this set
    My_t complement()const
    {
        My_t Temp(Universe_,{});
        for (const T& EI : *Universe_) {
            if(!this->count(EI))Temp.insert(EI);
        }
        return Temp;
    }

    /** \brief Returns true if this is a proper subset of other
     *
     *   This is a proper subset of RHS if they have the same universe,
     *   all elements in this are in RHS, and there is at least one
     *   element in RHS that is not in this
     *
     *   \param[in] RHS The set to comapre to
     *   \return True if this is a proper subset of other
     *
     */
    bool is_proper_subset_of(const My_t& RHS)const
    {
        return is_subset_of(RHS) && RHS.size() > this->size();
    }


    /** \brief Returns true if this is a subset of other
     *
     *   This is a subset of RHS if they have the same universe,
     *   and all elements in this are in RHS
     *
     *   \todo Avoid two equality checks (there's one in operator<)
     *
     *   \param[in] RHS The set to comapre to
     *   \return True if this is a subset of other
     *
     */
    bool is_subset_of(const My_t& RHS)const
    {
        if(!SameUniverse(RHS)) return false;
        for(const auto & it : *this)
            if(!RHS.count(it))
                return false;
        return true;
    }



    /** \brief Returns true if this is a proper superset of other
     *
     *  This is a proper superset of other, iff other is a proper subset
     *  of this.
     *
     *  \param[in] RHS Set to compare to
     *  \return True if this is a proper superset of other
     *
     */
    bool is_proper_superset_of(const My_t& RHS)const
    {
        return RHS.is_proper_subset_of(*this);
    }


    /** \brief Returns true if this is a superset of other
     *
     *  This is a superset of other iff other is a subset of this
     *
     *  \param[in] RHS Set to compare to
     *  \return true if this is a superset of other
     *
     */
    bool is_superset_of(const My_t& RHS)const
    {
        return RHS.is_subset_of(*this);
    }

    ///@}


    ///@{ \brief Set comparison operators


    /** \brief Returns true if this set equals other
     *
     *   Equality depends on two things the universe and which of the elements
     *   from the universe are present in this set.  For equality The universes
     *   must contain the same elements (as determined by Universe::operator==)
     *   and each set must contain the same elements
     *
     *   \param[in] RHS The other MathSet to compare to
     *   \return True if this set equals other
     */
    bool operator==(const My_t& RHS)const
    {
        return ((Universe_==RHS.Universe_||*Universe_==*RHS.Universe_)
                && Elems_ == RHS.Elems_);
    }

    /** \brief Returns true if this does not equal other
     *
     *   Literally just negates the result of operator==()
     *
     *   \param[in] RHS The other MathSet
     *   \return True if the sets are different
     *
     */
    bool operator!=(const My_t& RHS)const
    {
        return !(*this == RHS);
    }
    ///@}

        /** \brief Applies a function to each element in this set
     * 
     *  This function takes a function of the signature:
     *  \code
     *  T transformer(const T& elem_in);
     *  \endcode
     *  which given some element, returns that elements new value.
     *  
     * \param[in] transformer the function to apply
     * \return A new, non-aliased, trasformed version of this set
     */
    My_t transform(TransformerFunc transformer)const
    {
        //! \todo better way to do this function?
        //  This makes some assumptions about the ordering of elements
        std::shared_ptr<Universe_t> newuniverse(new Universe_t);

        for(size_t i = 0; i < Universe_->size(); ++i) {
            const auto & it = (*Universe_).at(i);

            if (Elems_.count(i) > 0)
                newuniverse->insert(transformer(it));
            else
                newuniverse->insert(it);
        }
        return My_t(newuniverse, this->Elems_);
    }

    /** \brief Partitions the set in two based on some criteria
     *
     *  This function creates a new set, with an aliased universe, that is
     *  made up of elements for which the selector function returns true.  The
     *  selector function should have the signature:
     *  \code
     *  bool Selector(const T& elem_in);
     *  \endcode
     *  and should return true if the element it was given is to be in the new
     *  set
     * 
     *  \param[in] selector the function used for selecting elements
     *  \return The selected elements as a set
     */
    My_t partition(SelectorFunc selector) const
    {
        std::set<size_t> newelems;
        for(const auto & idx : Elems_) {
            const auto & el = (*Universe_)[idx];
            if(selector(el))
                newelems.insert(idx);
        }
        return My_t(Universe_, newelems);
    }


    virtual std::string to_string()const
    {
        std::stringstream ss;
        for (const size_t& EI : Elems_)
            ss << (*Universe_)[EI] << " ";
        return ss.str();
    }

    /*! \brief obtain a unique hash of this MathSet
     * 
     * Hashes are the same if the Universes are equivalent
     * and if the elements are equivalent. The hashes will
     * be the same even if the two MathSets have different
     * Universes.
     */
    bphash::HashValue my_hash(void) const
    {
        return bphash::make_hash(bphash::HashType::Hash128, *this);
    }
    
private:

  
    ///The universe associated with this MathSet
    std::shared_ptr<const Universe_t> Universe_;
    
    ///Which elements of the universe are in the set
    std::set<size_t> Elems_;

    ///Does the universe contain the element
    void universe_contains(const T& Elem)const
    {
        if (!Universe_->count(Elem))
            throw ValueOutOfRange("Requested element is not in the universe");
    }
    
    ///Does the universe contain the index
    void universe_contains_idx(size_t Elem)const
    {
        if (Elem>=Universe_->size())
            throw ValueOutOfRange("Requested element is not in the universe");
    }

    /** \brief Determines if the universe of this and that of RHS are
     *   compatible
     *  
     *   In order to use this set as the left side of a set operation like
     *   union or intersection, the universe of this set must be the same as
     *   that of \pRHS or every element in \pRHS must be in this's universe,
     *   i.e. this's universe must be a superset of \pRHS's universe.
     */
    bool SameUniverse(const My_t& RHS) const
    {
       return Universe_==RHS.Universe_ || 
              Universe_->is_superset_of(*RHS.Universe_);
               //->is_superset_of(*RHS.Universe_);
    }

    //! \name Serialization and Hashing
    ///@{

    DECLARE_SERIALIZATION_FRIENDS
    BPHASH_DECLARE_HASHING_FRIENDS

    /* We have to split load/save since the
     * the shared_ptr points to const data, and
     * cereal can't serialize to const data
     */
    template<class Archive>
    void save(Archive & ar) const
    {
        ar(Universe_,Elems_);
    }

    template<class Archive>
    void load(Archive & ar)
    {
        std::shared_ptr<Universe_t> Newuniverse;
        ar(Newuniverse,Elems_);
        Universe_=std::move(Newuniverse);
    }

    void hash(bphash::Hasher & h) const
    {
        h(Universe_,Elems_);
    }

    ///@}


};

/*********************** Implementations **************************************/

template<typename T,typename U>
MathSet<T,U>& MathSet<T,U>::intersection_assign(const MathSet<T,U>& RHS)
{
        if(!SameUniverse(RHS))throw MathException("Universes are incompatible");
        if(&RHS==this)return *this;
        std::set<size_t> NewTemp;
        std::set_intersection(Elems_.begin(),Elems_.end(),
                RHS.Elems_.begin(), RHS.Elems_.end(),
                std::inserter<std::set<size_t>>(
                NewTemp, NewTemp.begin()));
        Elems_ = std::move(NewTemp);
        return *this;
}


template<typename T,typename U>
MathSet<T,U>& MathSet<T,U>::difference_assign(const MathSet<T,U>& RHS)
    {
        if(!SameUniverse(RHS))throw MathException("Universes are incompatible");
        if(&RHS==this){
            Elems_=std::set<size_t>();
            return *this;
        }
        std::set<size_t> NewTemp;
        std::set_difference(Elems_.begin(),Elems_.end(),
                RHS.Elems_.begin(), RHS.Elems_.end(),
                std::inserter<std::set < size_t >> (
                NewTemp, NewTemp.begin()));
        Elems_ = std::move(NewTemp);
        return *this;
    }

#define MATHSET_OP(op,name)\
template<typename T,typename U>\
MathSet<T,U> op(const MathSet<T,U>& LHS,const MathSet<T,U>& RHS) {\
   return LHS.name(RHS);\
}
#define MATHSET_OP2(op,name)\
template<typename T,typename U>\
MathSet<T,U>& op(MathSet<T,U>& LHS,const MathSet<T,U>& RHS) {\
   return LHS.name(RHS);\
}
#define MATHSET_COMP(op,name)\
template<typename T,typename U>\
bool op(const MathSet<T,U>& LHS,const MathSet<T,U>& RHS) {\
    return LHS.name(RHS);\
}

MATHSET_OP(operator+,set_union)
MATHSET_OP2(operator+=,union_assign)
MATHSET_OP(operator/,intersection)
MATHSET_OP2(operator/=,intersection_assign)
MATHSET_OP(operator-,difference)
MATHSET_OP2(operator-=,difference_assign)
MATHSET_COMP(operator<,is_proper_subset_of)
MATHSET_COMP(operator<=,is_subset_of)
MATHSET_COMP(operator>,is_proper_superset_of)
MATHSET_COMP(operator>=,is_superset_of)

#undef MATHSET_COMP
#undef MATHSET_OP2
#undef MATHSET_OP

}//End namespace pulsar

