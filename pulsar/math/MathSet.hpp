/*
 * File:   MathSet.hpp
 * Original Author: Ryan Richard <ryanmrichard1@gmail.com>
 *
 * Created on January 22, 2016, 11:11 AM
 */

#ifndef PULSAR_GUARD_MATH__MATHSET_HPP_
#define PULSAR_GUARD_MATH__MATHSET_HPP_

#include "pulsar/exception/Assert.hpp"
#include "pulsar/math/Universe.hpp"
#include "pulsar/util/IterTools.hpp"
#include <iterator>
namespace pulsar{
namespace math {

template<typename T,typename U> class MathSet;

/** An iterator to go with the MathSet class, returns actual objects
 *
 * The iterator only allows accessing the data, not modifying. That is,
 * this only behaves like a const_iterator
 */
template<typename T, typename U>
class ConstSetItr : public std::iterator<std::input_iterator_tag, const T> {
private:
    ///My type
    typedef ConstSetItr<T, U> My_t;
    typedef Universe<T,U> Universe_t;
    ///Type of the iterator to the Indices in the universe
    typedef std::set<size_t>::const_iterator Itr_t;
    ///The index I currently point to
    Itr_t CurrIdx_;
    ///The universe I am tied to
    const Universe_t* Set_;
    ///Let MathSet play with my private parts
    friend MathSet<T, U>;
    ///Only universe can make a working iterator
    ConstSetItr(Itr_t CurrIdx, const Universe_t* Set) :
        CurrIdx_(CurrIdx), Set_(Set) { }

public:
    ///Type being iterated over
    typedef T value_type;

    ///Resulting iterator is unusable unless it is set equal to usable version
    ConstSetItr()=default;
    ConstSetItr(const ConstSetItr&) = default;
    ConstSetItr& operator=(const ConstSetItr&) = default;
    ConstSetItr(ConstSetItr&&) = default;
    ConstSetItr& operator=(ConstSetItr&&) = default;
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
        return &((*Set_)[*CurrIdx_]);
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
    ///This class's type
    typedef MathSet<T, U> My_t;
public:
    typedef ConstSetItr<T, U> const_iterator;
    typedef T value_type;
    typedef U store_type;
    typedef std::function<bool(const T &) > SelectorFunc;
    typedef std::function<T(const T &)> TransformerFunc;
    typedef Universe<T, U> Universe_t;


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
    ///Makes a set that is part of the given universe
    // fill = Make this set a set of all elements in the universe
    MathSet(std::shared_ptr<const Universe_t> AUniverse, bool fill)
          : Universe_(AUniverse)
    {
            for(size_t i : util::Range<0>(fill?AUniverse->size():0))
                Elems_.insert(i);
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
    MathSet(const My_t&) = default;
    MathSet(My_t&&) = default;

    ///Returns a deep copy of everything
    My_t clone()const
    {
        return My_t(std::make_shared<Universe_t>(*Universe_), this->Elems_);
    }


    /// Obtain the universe used by this set
    std::shared_ptr<const Universe_t> get_universe(void) const
    {
        return Universe_;
    } 


    /*! \brief Obtain the contents of this MathSet as a new universe
     *
     * The new universe is not linked to this object in any way
     */
    Universe_t as_universe(void) const
    {
        Universe_t newuniverse;
        for(const auto & it : *this)
            newuniverse.push_back(it);
        return newuniverse;
    }   


    ///@}

    ///@{
    ///Basic accessors

    ///Returns the number of elements in this set
    size_t size(void)const noexcept
    {
        return Elems_.size();
    }

    const_iterator begin() const
    {
        return const_iterator(Elems_.begin(),Universe_.get());
    }

    const_iterator end() const
    {
        return const_iterator(Elems_.end(),Universe_.get());
    }


    ///Returns true if this set has the element
    bool count(const T& Elem)const{
        return (Universe_->count(Elem) &&
                count_idx(idx(Elem)) > 0);
    }

    bool count_idx(size_t Idx)const { return Elems_.count(Idx); }

    size_t idx(const T& Elem)const
    {
        // will throw if Elem is not part of our universe
        return Universe_->idx(Elem);
    }

    ///@}


    /// \name Set operations
    ///@{

    My_t& insert(const T & Elem)
    {
        universe_contains(Elem);
        Elems_.insert(idx(Elem));
        return *this;
    }
    
    My_t& insert_idx(size_t Idx)
    {
        universe_contains_idx(Idx);
        Elems_.insert(Idx);
        return *this;
    }

    My_t& union_assign(const My_t & RHS)
    {
        SameUniverse(RHS);
        Elems_.insert(RHS.Elems_.begin(), RHS.Elems_.end());
        return *this;
    }

    My_t set_union(const My_t& RHS)const
    {
        return My_t(*this).union_assign(RHS);
    }


    My_t& intersection_assign(const My_t& RHS);

    My_t intersection(const My_t & RHS) const
    {
        return My_t(*this).intersection_assign(RHS);
    }

    My_t& difference_assign(const My_t& RHS);

    My_t difference(const My_t& RHS) const
    {
        return My_t(*this).difference_assign(RHS);
    }


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
        if(Universe_ != RHS.Universe_) return false;
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


    /// \name Operator overloads
    ///@{

    /// \copydoc insert(const T& Elem)
    My_t & operator<<(const T& elem) { return insert(elem); }

    /// \copydoc insert(const T&& Elem)
    My_t & operator<<(T&& elem) { return insert(std::move(elem)); }

    /// \copydoc union_assign(const My_t & RHS)
    My_t& operator+=(const My_t& RHS) { return union_assign(RHS); }

    /// \copydoc union_assign(My_t&& RHS)
    My_t& operator+=(My_t&& RHS) { return union_assign(std::move(RHS)); }

    /// \copydoc set_union(My_t& RHS)
    My_t operator+(const My_t& RHS)const { return set_union(RHS); }

    /// \copydoc set_union(My_t&& RHS)
    My_t operator+(My_t&& RHS)const { return set_union(std::move(RHS)); }

    /// \copydoc intersection_assign(const My_t & rhs)
    My_t & operator/=(const My_t& RHS) { return intersection_assign(RHS); }

    /// \copydoc intersection(const My_t & rhs)
    My_t operator/(const My_t& RHS)const { return intersection(RHS); }

    /// \copydoc difference_assign(const My_t &)
    My_t & operator-=(const My_t& RHS) { return difference_assign(RHS); }

    /// \copydoc difference(const My_t &)
    My_t operator-(const My_t& RHS)const { return difference(RHS); }

    /// \copydoc is_proper_subset_of
    bool operator<(const My_t& RHS)const { return is_proper_subset_of(RHS); }

    /// \copydoc is_subset_of
    bool operator<=(const My_t& RHS)const { return is_subset_of(RHS); }

    /// \copydoc is_proper_superset_of
    bool operator>(const My_t& RHS)const { return is_proper_superset_of(RHS); }

    /// \copydoc is_superset_of
    bool operator>=(const My_t& RHS)const { return is_superset_of(RHS); }


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
        return (Universe_ == RHS.Universe_ && Elems_ == RHS.Elems_);
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

    void SameUniverse(const My_t& RHS) const
    {
        if(Universe_ != RHS.Universe_)
            throw MathException("Sets have different universes");
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

template<typename T,typename U>
MathSet<T,U>& MathSet<T,U>::intersection_assign(const MathSet<T,U>& RHS)
{
        SameUniverse(RHS);
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
        SameUniverse(RHS);
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


}//End namespace math
}//End namespace pulsar

#endif /* PULSAR_GUARD_MATHSET_HPP_ */

