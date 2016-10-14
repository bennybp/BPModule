#pragma once
/* 
 * \file   Universe.hpp
 * Original Author: richard
 * 
 * Created on January 27, 2016, 6:00 PM
 * 
 * \brief Definitions of a SetItr and a set Universe
 */

#include <memory> //Shared ptrs
#include <algorithm> //For std::find
#include <sstream> //For printing
#include <vector> //For default set container

#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/util/Serialization.hpp"
#include "bphash/Hasher.hpp"
#include "bphash/types/set.hpp"
#include "bphash/types/memory.hpp"
#include "bphash/types/vector.hpp"



namespace pulsar{
namespace math {

/** \brief A class that implements a mathematical ordered set.
 * 
 *   The Universe and MathSet classes are closesly related.  The main difference
 *   is that the Universe works directly with the elements whereas the MathSet
 *   works with proxies.  This makes the MathSet more effecient as it avoids
 *   copying the full objects and avoids having to call the (possibly) expensive
 *   comparison operators of the full objects.
 * 
 *  A Universe is an ordered set.  For the most part Universe acts like an
 *  std::vector with some additional functionality related to sets like union,
 *  intersection, uniqueness of elements, etc.
 * 
 *  \todo Is it better to switch to an std::unordered_map<T,size_t>, which maps
 *  each element to an arbitrary unique index?  Look-up and union would be
 *  constant.
 *
 * \par Hashing
 *     The hash value of a Universe is unique with respect to the values
 *     and the ordering in the container (if ordered). The hash will be
 *     the same even if the pointers point to different locations.
 * 
 *   \param T The type of the element.  Should be very light weight, will be
 *            copied a lot.  Must be default constructable and comparable with
 *            the less operator.  If not light weight consult the MathSet class.
 *   \param U The type of the storage class.  
 * 
 * \par Storage class concept
 *     The storage class must satisfy the typical STL API for an ordered
 *     container, notably:
 *         - Have a typedef iterator that is the type of the iterator
 *         - Have a typedef const_iterator that is the type of a constant
 *           iterator
 *         - Default constructable
 *         - Copy constructable
 *         - Constructable via initializer list
 *         - push_back function that appends an element to the end
 *  
 */
template<typename T, typename U = std::vector<T>>
class Universe {
private:

    ///My type
    typedef Universe<T, U> My_t;
    
    ///Where the actual elements are stored
    U Storage_;
    
    ///Checks an index
    void check_idx(size_t EI)const{
        if(EI >= size())
          throw ValueOutOfRange("Index is out of bounds", "index", EI);
    }

public:
    ///The type of the element
    using value_type=T;
    
    ///The type of a constant iterator to this class
    using const_iterator=typename U::const_iterator;
    
    ///The type of an iterator to this class
    using iterator=typename U::iterator;

    /// \name Constructors, destructors, assignment
    ///@{ 

    ///Makes an empty universe
    Universe(){ }
    
    ///Deep copies the universe
    Universe(const My_t& RHS) 
      : Storage_(RHS.Storage_) { }
    
    ///Creates universe that contains elements in constructor (attempts to use
    ///different types will fail to initialize U)
    template<typename...Args>
    Universe(T arg1,Args...args)
      : Storage_({arg1,args...}){}
       
    ///Creates universe that contains elements in initializer list
    Universe(std::initializer_list<T> l)
      :Storage_(l){}

    
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
    ///@}


    ///@{
    ///Basic accessors
    
    //Returns the cardinality of the universe (i.e. the number of elements)
    size_t size()const noexcept
    {
        return Storage_.size();
    }

    ///Returns a const iterator to the beginning of the universe 
    const_iterator begin()const
    {
        return Storage_.begin();
    }

    ///Returns an iterator just past a const version of the last element
    const_iterator end()const
    {
        return Storage_.end();
    }

    ///Returns true if this set contains Elem, comparison occurs via
    ///Elem's operator==
    bool count(const T& Elem)const
    {
        return std::find(begin(),end(), Elem) != end();
    }
    
    bool count_idx(size_t Elem)const
    {
        return Elem<Storage_.size();
    }

    ///Returns the index of an Elem in Storage_
    size_t idx(const T& Elem)const
    {
        auto it = std::find(begin(), end(), Elem);
        if(it != end())
            return std::distance(begin(), it);
        else 
            throw ValueOutOfRange("Element is not part of this universe");
    }


    const T& operator[](size_t EI)const { return at(EI); }
    //T& operator[](size_t EI){return at(EI);}

    
    const T& at(size_t EI) const
    {
        check_idx(EI);
        return Storage_.at(EI);
    }
    
    /*T& at(size_t EI)
    {
        check_idx(EI);
        return Storage_.at(EI);
    }*/

    ///@}


    /// \name Set operations
    ///@{


    /** \brief Adds an element to this universe
     * 
     * This function adds \p elem to the universe if it is not already
     * present.  \p elem will be copied.
     * 
     * \note this function (may) invalidate all iterators that are out
     */
    void push_back(const T& elem)
    {
        if(!count(elem))
            Storage_.push_back(elem); // actually copy the data
    }
    
    Universe<T,U>& insert(const T& elem){
        push_back(elem);
        return *this;
    }

     /** \brief Moves \p elem to this universe
     * 
     * This function moves \p elem into this universe iff the element is not
     * already in the universe.
     * 
     * \note this function (may) invalidate all iterators that are out
     */
    void push_back(T&& elem)
    {
        if(!count(elem))
            Storage_.push_back(std::move(elem));
    }
    
    Universe<T,U>& insert(T&& elem)
    {
        push_back(std::move(elem));
        return *this;
    }

    template<typename BeginItr,typename EndItr>
    Universe<T,U>& insert(BeginItr b,EndItr e)
    {
        while(b!=e)push_back(*b++);
        return *this;
    }
    
    /** \brief Makes this the union of this and \p RHS
     * 
     *  This function appends the unique elements of \p RHS to this function.
     * 
     *  \note In general the data in Storage_ is not sorted so we can't use
     *  std::set_union.
     */
    My_t& union_assign(const My_t& RHS)
    {
        for (const T & EI : RHS)push_back(EI);
        return *this;
    }


    My_t& union_assign(My_t&& RHS)
    {
        for(T& EI : RHS.Storage_)push_back(std::move(EI));
        return *this;
    }


    ///Returns a new universe that is union of this and \p RHS
    My_t set_union(const My_t& RHS)const
    {
        return My_t(*this).union_assign(RHS);
    }

    ///Returns a new universe (not linked to this one) that is union of this and
    /// \p RHS
    My_t set_union(My_t&& RHS)const
    {
        return My_t(*this).union_assign(std::move(RHS));
    }

    /** \brief Makes this the intersection of this and RHS
     * 
     *  Again, we don't want to sort and so we can't use std::set_intersection.
     *  The elements contained in this after the operation, will be in the same
     *  order they were  originally.
     */
    My_t& intersection_assign(const My_t& RHS)
    {
        U Temp;
        for (T& Element: Storage_) 
            if (RHS.count(Element)) 
                Temp.push_back(std::move(Element));
        Storage_ = std::move(Temp);
        return *this;
    }


    /*! \brief Returns the intersection of this and RHS */
    My_t intersection(const My_t& RHS) const
    {
        return My_t(*this).intersection_assign(RHS);
    }

    /** \brief Makes this the set difference of this and RHS
     * 
     *  Again, we can't sort Storage_, so we can't use std::set_difference. As
     *  with intersection, the final order is the same as it was orginally
     */
    My_t& difference_assign(const My_t& RHS)
    {
        U Temp;
        for (T& Element: Storage_) 
            if (!RHS.count(Element))
                Temp.push_back(std::move(Element));
        Storage_ = std::move(Temp);
        return *this;
    }

    My_t difference(const My_t& RHS) const
    {
        return My_t(*this).difference_assign(RHS);
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
    bool is_proper_subset_of(const My_t& RHS)const
    {
        // Are we a subset, and does RHS have more elements than we do?
        return is_subset_of(RHS) && RHS.size() > this->size();
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
    bool is_subset_of(const My_t& RHS)const
    {
        for(const auto & it : *this)
            if(!RHS.count(it))
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

    /// \copydoc union(My_t& RHS)
    My_t operator+(const My_t& RHS)const { return set_union(RHS); }

    /// \copydoc union(My_t&& RHS)
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
     *   We define equality of universe as having the same elements in
     *   storage (by value) and the same elements
     * 
     *   \param[in] RHS The other universe to compare to
     *   \return True if this set equals other
     */
    bool operator==(const My_t& RHS)const
    {
        // Check if we have the same number of elements
        if(size() != RHS.size())
            return false;
        // Go element by element
        for(const auto & it : *this)
            if(!RHS.count(it))
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
    std::string to_string()const
    {
        std::stringstream ss;
        for (const T & EI : * this)ss << EI << " ";
        return ss.str();
    }


    /* \brief Return a hash of this universe
     * 
     * Hashes are the same if the held data is equivalent (ie,
     * same values and same elements)
     */
    bphash::HashValue my_hash(void) const
    {
        return bphash::make_hash(bphash::HashType::Hash128, *this);
    }
    
    private:
    //! \name Serialization and Hashing
    ///@{
    
    DECLARE_SERIALIZATION_FRIENDS
    BPHASH_DECLARE_HASHING_FRIENDS

    /* We have to split load/save since MathSet uses
     * load/save, and these are inherited. If not,
     * cereal will find serialize() here and load/save
     * for MathSet, and trigger an assertion
     */
    template<class Archive>
    void save(Archive & ar) const
    {
        ar(Storage_);
    }
    
    template<class Archive>
    void load(Archive & ar)
    {
        ar(Storage_);
    }

    void hash(bphash::Hasher & h) const
    {
        h(Storage_);
    }

    ///@}
};

template<typename T, typename U>
std::ostream& operator<<(std::ostream& os, const Universe<T, U>& AUniv)
{
    return os << AUniv.to_string();
}

}
}//End namespaces

