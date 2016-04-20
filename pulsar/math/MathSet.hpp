/*
 * File:   MathSet.hpp
 * Original Author: Ryan Richard <ryanmrichard1@gmail.com>
 *
 * Created on January 22, 2016, 11:11 AM
 */

#ifndef PULSAR_GUARD_MATH__MATHSET_HPP_
#define PULSAR_GUARD_MATH__MATHSET_HPP_

#include <vector> //For default set container
#include "pulsar/exception/Assert.hpp"
#include "pulsar/math/Universe.hpp"

namespace pulsar{
namespace math {

/** \brief A class for performing set manipulations efficiently relative to
 *   some universe.
 *
 *   With the universe class all of the operations are deep copies or involve
 *   copying the actual elements over.  Now that we know what our universe is,
 *   we can do these operations much more efficiently by mapping each element
 *   to an integer in the range 0 to the size of our universe and performing
 *   the operations on those integers.  That's what this class does.  Thus
 *   by default operations with this class are shallow copies.  It is entirely
 *   possible to simply use the Universe class for set manipulations (barring
 *   complement), it will just not be as efficient.
 *
 *   Because a MathSet derives from Universe it is perfectly capable of being
 *   itself used as a universe.  Note that the privateness of the Universe
 *   base class means you can't pass this class by it's base class to anything
 *   other than this class.  All classes that intend to use MathSet should
 *   derive from it.
 *
 *   Developer note:  I originally thought we could get away without the
 *   shared_ptr to the Universe_.  When we link to a Universe, we just set
 *   the Storage_ in the base class to that of the one in Universe we are
 *   linking to.  Then we can use the base class's interface to do everything.
 *   This works fine if we don't ever intend to use a MathSet as a universe, but
 *   we do.  Imagine we have:
 *   \code
 *    Universe U;
     MathSet A(U);
 *    MathSet B(A);
 *   \endcode
 *   where we assume the objects are actually declared and setup correctly,
 *   additionally we assume A is a proper subset of U. Now how do we know if an
 *   element is in B's Universe?  For A we just had to check if it was in
 *   Storage_, but for B this won't work because Storage_ hasn't changed to
 *   represent the reduced subspace (we could do this, but then we'd have to
 *   copy and the point of this class is to avoid that (the Universe class
 *   would have taken that route)).  However, by storing a pointer to A, we
 *   need only compare against A's Elems_.
 *
 *   \param T The type of the object in the set
     \param U The type of the container holding the objects
 *
 */
template<typename T, typename U = std::vector<T>>
class MathSet : private Universe<T, U> {
private:
    ///Base class's type
    typedef Universe<T, U> Base_t;
    ///This class's type
    typedef MathSet<T, U> My_t;

    std::shared_ptr<const Base_t> Universe_;

    void UniverseContains(const T& Elem)const
    {
        if (!Universe_->Contains(Elem))
            throw exception::ValueOutOfRange("Requested element is not in the universe for this set");
    }

    void UniverseContainsIdx(size_t Idx)const
    {
        if (!Universe_->ContainsIdx(Idx))
            throw exception::ValueOutOfRange("Requested element is not in the universe for this set");
    }

    void SameUniverse(const My_t& RHS) const
    {
        if(Universe_ != RHS.Universe_)
            throw exception::MathException("Operation being performed on sets with different universes");
    }

    // constructs via shared pointer to universe and a given
    // set of elements
    MathSet(std::shared_ptr<const Base_t> Universe,
            const std::set<size_t> & Elems)
    : Universe_(Universe)
    {
        this->Elems_ = Elems;
    }

    //! \name Serialization
    ///@{

    DECLARE_SERIALIZATION_FRIENDS

    /* We have to split load/save since the
     * the shared_ptr points to const data, and
     * cereal can't serialize to const data
     */
    template<class Archive>
    void save(Archive & ar) const
    {
        ar(cereal::base_class<Base_t>(this), Universe_);
    }

    template<class Archive>
    void load(Archive & ar)
    {
        std::shared_ptr<Base_t> Newuniverse;
        ar(cereal::base_class<Base_t>(this), Newuniverse);
        Universe_=std::move(Newuniverse);
    }

    ///@}



public:
    typedef ConstSetItr<T, U> const_iterator;
    typedef T value_type;
    typedef U store_type;
    typedef std::function<bool(const T &) > SelectorFunc;
    typedef std::function<T(const T &)> TransformerFunc;

    typedef Universe<T, U> Universe_t;


    /// \name Constructors, destructors, assignment
    ///@{

    ///Makes a set that is part of the given universe
    // fill = Make this set a set of all elements in the universe
    MathSet(std::shared_ptr<const Base_t> AUniverse, bool fill)
          : Universe_(AUniverse)
    {
        if (fill) {
            this->Elems_ = AUniverse->Elems_;
        }
    }


    /*! \brief For serialization only
     *
     * \warning NOT FOR USE OUTSIDE OF SERIALIZATION
     * \todo Replace if cereal fixes this
     */
    MathSet() {} // cannot be "=default" due to compiler bugs
                 // (symbol may be missing when compiled in Debug mode)



    ///Deep copies elements, shallow copies Universe_ and (and Storage_)
    My_t& operator=(const My_t &) = default;
    MathSet & operator=(My_t&&) = default;
    MathSet(const My_t&) = default;
    MathSet(My_t&&) = default;

    ///Returns a deep copy of everything
    virtual My_t Clone()const
    {
        return My_t(std::shared_ptr<Base_t>(new Base_t(*Universe_)), this->Elems_);
        //                                      ^^ Deep copy universe, copy elements
    }


    ///@}

    ///@{
    ///Basic accessors

    ///Returns the number of elements in this set
    size_t Size(void)const noexcept
    {
        return this->Elems_.size();
    }

    const_iterator begin() const
    {
        return const_iterator(Universe_->MakeIterator(this->Elems_.begin()));
    }

    const_iterator end() const
    {
        return const_iterator(Universe_->MakeIterator(this->Elems_.end()));
    }


    ///Returns true if this set has the element
    bool Contains(const T& Elem)const{
        return (Universe_->Contains(Elem) &&
                ContainsIdx(Idx(Elem)) > 0);
    }

    bool ContainsIdx(size_t Idx)const { return this->Elems_.count(Idx); }

    size_t Idx(const T& Elem)const
    {
        // will throw if Elem is not part of our universe
        return Universe_->Idx(Elem);
    }

    ///@}


    /// \name Set operations
    ///@{

    My_t& Insert(const T & Elem)
    {
        UniverseContains(Elem);
        this->Elems_.insert(Idx(Elem));
        return *this;
    }

    My_t& InsertIdx(size_t Idx)
    {
        UniverseContainsIdx(Idx);
        this->Elems_.insert(Idx);
        return *this;
    }

    My_t& UnionAssign(const My_t & RHS)
    {
        SameUniverse(RHS);
        this->Elems_.insert(RHS.Elems_.begin(), RHS.Elems_.end());
        return *this;
    }

    My_t Union(const My_t& RHS)const
    {
        return My_t(*this).UnionAssign(RHS);
    }


    My_t& IntersectionAssign(const My_t& RHS)
    {
        SameUniverse(RHS);
        // careful, RHS may be the same object as this
        std::set<size_t> NewTemp;
        std::set_intersection(this->Elems_.begin(), this->Elems_.end(),
                RHS.Elems_.begin(), RHS.Elems_.end(),
                std::inserter<std::set<size_t>>(
                NewTemp, NewTemp.begin()));
        this->Elems_ = std::move(NewTemp);
        return *this;
    }

    My_t Intersection(const My_t & RHS) const
    {
        return My_t(*this).IntersectionAssign(RHS);
    }

    My_t& DifferenceAssign(const My_t& RHS)
    {
        SameUniverse(RHS);
        // careful, RHS may be the same object as this
        std::set<size_t> NewTemp;
        std::set_difference(this->Elems_.begin(), this->Elems_.end(),
                RHS.Elems_.begin(), RHS.Elems_.end(),
                std::inserter<std::set < size_t >> (
                NewTemp, NewTemp.begin()));
        this->Elems_ = std::move(NewTemp);
        return *this;
    }

    My_t Difference(const My_t& RHS) const
    {
        return My_t(*this).DifferenceAssign(RHS);
    }


    My_t Complement()const
    {
        My_t Temp(Universe_,{});
        for (const T& EI : *Universe_) {
            if(!this->Contains(EI))Temp.Insert(EI);
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
    bool IsProperSubsetOf(const My_t& RHS)const
    {
        return IsSubsetOf(RHS) && RHS.Size() > this->Size();
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
    bool IsSubsetOf(const My_t& RHS)const
    {
        if(Universe_ != RHS.Universe_) return false;
        for(const auto & it : *this)
            if(!RHS.Contains(it))
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
     *   We define equality as having literally the same universe,
     *   i.e. the pointers to the Universes have to be the same, and
     *   having the same elements.
     *
     *   \param[in] RHS The other MathSet to compare to
     *   \return True if this set equals other
     */
    bool operator==(const My_t& RHS)const
    {
        return (Universe_ == RHS.Universe_ && this->Elems_ == RHS.Elems_);
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



    My_t Transform(TransformerFunc transformer)const
    {
        //! \todo better way to do this function?
        //  This makes some assumptions about the ordering of elements
        std::shared_ptr<Base_t> newuniverse(new Base_t);

        for(size_t i = 0; i < Universe_->Size(); ++i) {
            const auto & it = (*Universe_).At(i);

            if (this->Elems_.count(i) > 0)
                newuniverse->Insert(transformer(it));
            else
                newuniverse->Insert(it);
        }
        return My_t(newuniverse, this->Elems_);
    }

    My_t Partition(SelectorFunc selector) const
    {
        std::set<size_t> newelems;
        for(const auto & idx : this->Elems_) {
            const auto & el = (*Universe_)[idx];
            if(selector(el))
                newelems.insert(idx);
        }
        return My_t(Universe_, newelems);
    }


    virtual std::string ToString()const
    {
        std::stringstream ss;
        for (const size_t& EI : this->Elems_)
            ss << (*Universe_)[EI] << " ";
        return ss.str();
    }

};

}
}//End namespaces

#endif /* PULSAR_GUARD_MATHSET_HPP_ */

