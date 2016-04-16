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
#include <iterator> //std::iterator

#include "bpmodule/util/Serialization.hpp"
#include "bpmodule/exception/Exceptions.hpp"


namespace bpmodule {
namespace math {

// Forward declarations for friends
template<typename T, typename U>
class Universe;
template<typename T, typename U>
class MathSet;

/** An iterator to go with the universe class, returns actual objects
 *
 * The iterator only allows accessing the data, not modifying. That is,
 * this only behaves like a const_iterator
 */
template<typename T, typename U>
class ConstSetItr : public std::iterator<std::input_iterator_tag, const T> {
private:
    ///My type
    typedef ConstSetItr<T, U> My_t;
    ///Type of the iterator to the Indices in the universe
    typedef std::set<size_t>::const_iterator Itr_t;
    ///The index I currently point to
    Itr_t CurrIdx_;
    ///The universe I am tied to
    const U* Set_;
    ///Let universe play with my private parts
    friend Universe<T, U>;
    ///Only universe can make a working iterator
    ConstSetItr(Itr_t CurrIdx, const U& Set) :
    CurrIdx_(CurrIdx), Set_(&Set) { }

public:
    ///Type being iterated over
    typedef T value_type;

    ConstSetItr(const ConstSetItr&) = default;
    ConstSetItr& operator=(const ConstSetItr&) = default;
    ~ConstSetItr() = default;

    ///Returns true if this iterator is equal to RHS
    bool operator==(const My_t& RHS)const
    {
        return (CurrIdx_ == RHS.CurrIdx_ &&
                Set_ == RHS.Set_);
    }

    ///Returns true if this iterator is not equal to RHS
    bool operator!=(const My_t& RHS)const
    {
        return !this->operator==(RHS);
    }

    ///Returns a reference to the current element
    const T & operator*()const
    {
        return (*Set_)[*CurrIdx_];
    }

    const T * operator->()const
    {
        return &(*this);
    }

    ///Prefix increment operator
    My_t& operator++()
    {
        ++CurrIdx_;
        return *this;
    }

    ///Postfix increment operator
    My_t operator++(int)
    {
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
 *            actual memory for the elements.  
 *            Note that std::vector satisfies these requests and is the default
 */
template<typename T, typename U = std::vector<T>>
class Universe {
protected:
    friend MathSet<T, U>;

    ///My type
    typedef Universe<T, U> My_t;
    ///The storage class
    std::shared_ptr<U> Storage_;
    ///The elements in this set
    std::set<size_t> Elems_;

    ///For iterating over MathSet and other derived classes
    ConstSetItr<T, U> MakeIterator(std::set<size_t>::const_iterator ElemIt) const
    {
        return ConstSetItr<T, U>(ElemIt, *Storage_);
    }


private:
    //! \name Serialization
    ///@{
    
    DECLARE_SERIALIZATION_FRIENDS

    /* We have to split load/save since MathSet uses
     * load/save, and these are inherited. If not,
     * cereal will find serialize() here and load/save
     * for MathSet, and trigger an assertion
     */
    template<class Archive>
    void save(Archive & ar) const
    {
        ar(Storage_, Elems_);
    }
    
    template<class Archive>
    void load(Archive & ar)
    {
        ar(Storage_, Elems_);
    }

    ///@}

public:
    typedef T value_type;

    ///An iterator to const versions of the elements in this set
    typedef ConstSetItr<T, U> const_iterator;

    /// \name Constructors, destructors, assignment
    ///@{ 

    ///Makes an empty universe
    Universe() : Storage_(new U) { };
    ///Move construct
    Universe(My_t &&) = default;
    ///Deep copies the universe
    Universe(const My_t& RHS) 
      : Storage_(new U(*RHS.Storage_)), Elems_(RHS.Elems_) { }
    ///Deep copies during assignment
    My_t& operator=(const My_t & RHS)
    {
        using std::swap;
        if (this != &RHS) {
            My_t tmp(RHS);
            swap(*this, tmp);
        }
        return *this;
    }

    ///Move assignment
    My_t & operator=(My_t &&) = default;

    ///No special memory clean-up
    // Cannot be =default due to compiler bugs
    /// \todo ^^ Is this true? ^^
    ~Universe() { };

    ///@}


    ///@{
    ///Basic accessors

    ///Returns the cardinality of the universe (i.e. the number of elements)
    size_t Size()const noexcept
    {
        return Elems_.size();
    }

    ///Returns a const iterator to the beginning of the universe 
    const_iterator begin()const
    {
        return const_iterator(Elems_.begin(), *Storage_);
    }

    ///Returns an iterator just past a const version of the last element
    const_iterator end()const
    {
        return const_iterator(Elems_.end(), *Storage_);
    }

    ///Returns true if this set contains Elem, comparison occurs via
    ///Elem's operator==
    bool Contains(const T& Elem)const
    {
        auto it = std::find(Storage_->begin(), Storage_->end(), Elem);
        if(it != Storage_->end())
            return true;
        else 
            return false;
    }

    ///Returns true if this set contains an element with the given index
    bool ContainsIdx(size_t EI)const
    {
        return Elems_.count(EI) > 0;
    }


    ///Returns the index of an Elem in Storage_
    size_t Idx(const T& Elem)const
    {
        auto it = std::find(Storage_->begin(), Storage_->end(), Elem);
        if(it != Storage_->end())
            return std::distance(Storage_->begin(), it);
        else 
            throw exception::ValueOutOfRange("Element is not part of this universe");
    }


    const T& operator[](size_t EI)const { return (*Storage_)[EI]; }


    const T& At(size_t EI) const
    {
        if(EI >= Storage_->size())
            throw exception::ValueOutOfRange("Out of bounds access in universe", "index", EI);
        return (*this)[EI];
    }

    ///@}


    /// \name Set operations
    ///@{


    /** \brief Adds an element to this universe, returns this
     * 
     * This function adds an element to the universe and allocates
     * memory for it in the storage class.  If the storage class
     * is not allocated this function allocates it.  Ultimately, all
     * insertion calls go through this function.
     *
     * The data from \p elem is copied
     * 
     * Note: calling this function invalidates all iterators that are
     * out
     */
    My_t& Insert(const T& Elem)
    {
        if(!Contains(Elem))
        {
            Elems_.insert(Storage_->size()); // add the index
            Storage_->insert(Storage_->end(), Elem); // actually copy the data
        }
        return *this;
    }

    
     /** \brief Moves an element to this universe
     * 
     * This function adds an element to the universe and allocates
     * memory for it in the storage class.  If the storage class
     * is not allocated this function allocates it.  Ultimately, all
     * insertion calls go through this function.
     *
     * The data from \p elem is moved to the storage
     * 
     * Note: calling this function invalidates all iterators that are
     * out
     */
    My_t& Insert(T&& Elem)
    {
        if(!Contains(Elem))
        {
            Elems_.insert(Storage_->size()); // add the index
            Storage_->insert(Storage_->end(), std::move(Elem)); // actually move the data
        }
        return *this;
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
    My_t& UnionAssign(const My_t& RHS)
    {
        for (const T & EI : RHS)
            if (!this->Contains(EI))(*this) << EI;
        return *this;
    }


    My_t& UnionAssign(My_t&& RHS)
    {
        for(T & EI : *RHS.Storage_)
            if (!this->Contains(EI))(*this) << std::move(EI);
        return *this;
    }


    ///Returns a new universe (not linked to this one) that is union of this and
    ///RHS
    My_t Union(const My_t& RHS)const
    {
        return My_t(*this).UnionAssign(RHS);
    }

    ///Returns a new universe (not linked to this one) that is union of this and
    ///RHS
    My_t Union(My_t&& RHS)const
    {
        return My_t(*this).UnionAssign(std::move(RHS));
    }

    /** \brief Makes this the intersection of this and RHS
     * 
     *  Again, we don't want to sort and so we can't use std::set_intersection.
     *  The elements contained in this after the operation, will be in the same
     *  order they were  originally.
     */
    My_t& IntersectionAssign(const My_t& RHS)
    {
        std::shared_ptr<U> Temp(new U);
        std::set<size_t> TempElems;
        for (size_t EI : Elems_) {
            T & Element = Storage_->at(EI);
            if (RHS.Contains(Element)) {
                TempElems.insert(Temp->size());
                Temp->insert(Temp->end(), std::move(Element));
            }
        }
        Storage_ = std::move(Temp);
        Elems_ = std::move(TempElems);
        return *this;
    }


    /*! \brief Returns the intersection of this and RHS */
    My_t Intersection(const My_t& RHS) const
    {
        return My_t(*this).IntersectionAssign(RHS);
    }

    /** \brief Makes this the set difference of this and RHS
     * 
     *  Again, we can't sort Storage_, so we can't use std::set_difference. As
     *  with intersection, the final order is the same as it was orginally
     */
    My_t& DifferenceAssign(const My_t& RHS)
    {
        std::shared_ptr<U> Temp(new U);
        std::set<size_t> TempElems;
        for (size_t EI : Elems_) {
            T & Element = Storage_->at(EI);
            if (!RHS.Contains(Element)) {
                TempElems.insert(Temp->size());
                Temp->insert(Temp->end(), std::move(Element));
            }
        }
        Storage_ = std::move(Temp);
        Elems_ = std::move(TempElems);
        return *this;
    }

    My_t Difference(const My_t& RHS) const
    {
        return My_t(*this).DifferenceAssign(RHS);
    }

    /** \brief Returns true if this is a proper subset of other
     * 
     *   This is a proper subset of RHS if
     *   all elements in this are in RHS, and there is at least one
     *   element in RHS that is not in this
     *  
     *   \todo Implement early abort instead of using intersection
     * 
     *   \param[in] RHS The set to comapre to
     *   \return True if this is a proper subset of other
     *
     */
    bool IsProperSubsetOf(const My_t& RHS)const
    {
        // Are we a subset, and does RHS have more elements than we do?
        //! \todo is this logic correct?
        return IsSubsetOf(RHS) && RHS.Size() > this->Size();
    }

    /** \brief Returns true if this is a subset of other
     * 
     *   This is a subset of RHS if all elements in this are in RHS
     *  
     *   \todo Avoid two equality checks (there's one in operator<)
     * 
     *   \param[in] RHS The set to comapre to
     *   \return True if this is a subset of other
     *
     */
    bool IsSubsetOf(const My_t& RHS)const
    {
        for(const auto & it : *this)
            if(!RHS.Contains(it))
                return false; // we have an element not in RHS
        return true; // All our elements are in RHS
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
    bool IsProperSupersetOf(const My_t& RHS)const
    {
        return RHS.IsProperSubsetOf(*this);
    }

    /** \brief Returns true if this is a superset of other
     * 
     *  This is a superset of other iff other is a subset of this
     * 
     *  \param[in] RHS Set to compare to
     *  \return true if this is a superset of other
     *  
     */
    bool IsSupersetOf(const My_t& RHS)const
    {
        return RHS.IsSubsetOf(*this);
    }

    ///@}


    /// \name Operator overloads
    ///@{

    /// \copydoc Insert(const T& Elem)
    My_t & operator<<(const T& elem) { return Insert(elem); }

    /// \copydoc Insert(const T&& Elem)
    My_t & operator<<(T&& elem) { return Insert(std::move(elem)); }

    /// \copydoc UnionAssign(const My_t & RHS)
    My_t& operator+=(const My_t& RHS) { return UnionAssign(RHS); }

    /// \copydoc UnionAssign(My_t&& RHS)
    My_t& operator+=(My_t&& RHS) { return UnionAssign(std::move(RHS)); }

    /// \copydoc Union(My_t& RHS)
    My_t operator+(const My_t& RHS)const { return Union(RHS); }

    /// \copydoc Union(My_t&& RHS)
    My_t operator+(My_t&& RHS)const { return Union(std::move(RHS)); }

    /// \copydoc IntersectionAssign(const My_t & rhs)
    My_t & operator/=(const My_t& RHS) { return IntersectionAssign(RHS); }

    /// \copydoc Intersection(const My_t & rhs)
    My_t operator/(const My_t& RHS)const { return Intersection(RHS); }

    /// \copydoc DifferenceAssign(const My_t &)
    My_t & operator-=(const My_t& RHS) { return DifferenceAssign(RHS); }

    /// \copydoc Difference(const My_t &)
    My_t operator-(const My_t& RHS)const { return Difference(RHS); }

    /// \copydoc IsProperSubsetOf
    bool operator<(const My_t& RHS)const { return IsProperSubsetOf(RHS); }

    /// \copydoc IsSubsetOf
    bool operator<=(const My_t& RHS)const { return IsSubsetOf(RHS); }

    /// \copydoc IsProperSupersetOf
    bool operator>(const My_t& RHS)const { return IsProperSupersetOf(RHS); }

    /// \copydoc IsSupersetOf
    bool operator>=(const My_t& RHS)const { return IsSupersetOf(RHS); }

    ///@}


    ///@{ \brief Set comparison operators

    /** \brief Returns true if this set equals other
     * 
     *   We define equality of universe as having the same elements in
     *   storage (by value) and the same elements
     * 
     *   \param[in] RHS The other universe to compare to
     *   \return True if this set equals other
     */
    bool operator==(const My_t& RHS)const
    {
        // Check pointers first so we can skip elementwise comparison if we can
        if(Storage_ == RHS.Storage_)
            return true;
        // Check if we have the same number of elements
        if(Size() != RHS.Size())
            return false;
        // Go element by element
        for(const auto & it : *this)
            if(!RHS.Contains(it))
                return false;
        // reverse (may not be needed if elements are guaranteed to be unique,
        // but to be safe
        for(const auto & it : RHS)
            if(!Contains(it))
                return false;

        return true;
    }

    /** \brief Returns true if this does not equal other
     * 
     *   Literally just negates the result of operator==()
     * 
     *   \param[in] RHS The other MathSet
     *   \return True if the sets are different
     */
    bool operator!=(const My_t& RHS)const
    {
        return !(*this == RHS);
    }

    ///@}

    ///Helpful printing function
    //! \todo is this needed? Using this requires the stored type to have << overloaded
    std::string ToString()const
    {
        std::stringstream ss;
        for (const T & EI : * this)ss << EI << " ";
        return ss.str();
    }
};

template<typename T, typename U>
std::ostream& operator<<(std::ostream& os, const Universe<T, U>& AUniv)
{
    return os << AUniv.ToString();
}

}
}//End namespaces
#endif /* BPMODULE_GUARD_Universe_HPP_ */

