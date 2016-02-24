/* 
 * File:   MathSet.hpp
 * Original Author: Ryan Richard <ryanmrichard1@gmail.com>
 *
 * Created on January 22, 2016, 11:11 AM
 */

#ifndef BPMODULE_GUARD_MATH__MATHSET_HPP_
#define BPMODULE_GUARD_MATH__MATHSET_HPP_

#include <vector> //For default set container
#include <algorithm> //For set operations
#include "bpmodule/exception/Assert.hpp"
#include "bpmodule/exception/Exceptions.hpp"
#include "bpmodule/math/Universe.hpp"

namespace bpmodule {
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
         *    MathSet A(U);
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
         */
        template<typename T, typename U = std::vector<T>>
        class MathSet : private Universe<T, U> {
        private:
            ///Base class's type
            typedef Universe<T, U> Base_t;
            ///This class's type
            typedef MathSet<T, U> My_t;

            std::shared_ptr<const Base_t> Universe_;

            ///Checks if Elem (either as type T or as size_t) is in the universe
            template<typename V>
            void InUniverse(const V& Elem)const {
                exception::Assert<exception::ValueOutOfRange>(
                        Universe_->Contains(Elem),
                        "Requested element is not in the universe for this set"
                        );
            }

        protected:
            MathSet() = default;

        public:

            typedef T value_type;
            typedef U store_type;

            ///Makes a set that is part of the given universe
            // fill = Make this set a set of all elements in the universe
            MathSet(std::shared_ptr<const Base_t> AUniverse, bool fill) : Universe_(AUniverse)
            {
                //!\todo replace with getting a set of indicies from the universe? Would probably be safer
                if(fill)
                {
                    for(size_t i = 0; i < AUniverse->size(); i++) this->Elems_.insert(i);
                }
            }


            ///Deep copies elements, shallow copies Universe_ and Storage_
            MathSet(const My_t&) = default;
            MathSet(My_t&&) = default;
            MathSet & operator=(My_t&&) = default;

            ///Returns a deep copy of everything
            virtual My_t Clone()const {
                My_t Temp;
                //Deep copies this's member data
                Temp.Base_t::operator=(*this);
                //Deep copies the universe
                Temp.Universe_ = std::shared_ptr<Base_t>(new Base_t(*Universe_));
                return Temp;
            }


            size_t size(void)const noexcept {
                return this->Elems_.size();
            }

            virtual size_t Idx(const T& Elem)const {
                return Universe_->Idx(Elem);
            }


            ///Same as copy constructor, but for assignment
            My_t& operator=(const My_t &) = default;

            ///For adding an element, if you know its index in the universe
            My_t& operator<<(size_t Idx) {
                InUniverse(Idx);
                this->Elems_.insert(Idx);
                return *this;
            }

            ///Unlike the base class, we just add the index
            virtual My_t& operator<<(const T& Elem) {
                InUniverse(Elem);
                return (*this) << Universe_->Idx(Elem);
            }

            ///Makes this the union of this and other
            const My_t& operator+=(const My_t& RHS) {
                for (const size_t& EI : RHS.Elems_)
                    this->Elems_.insert(EI);
                return *this;
            }

            ///Returns the union of this and other (result shares same resources)
            My_t operator+(const My_t& RHS)const {
                return My_t(*this) += RHS;
            }

            ///Makes this the intersection of this and other
            const My_t& operator/=(const My_t& RHS) {
                std::set<size_t> Temp(std::move(this->Elems_));
                this->Elems_ = std::set<size_t>();
                std::set_intersection(Temp.begin(), Temp.end(),
                        RHS.Elems_.begin(), RHS.Elems_.end(),
                        std::inserter<std::set < size_t >> (
                        this->Elems_, this->Elems_.begin()));
                return *this;
            }

            ///Returns the intersection of this and other
            My_t operator/(const My_t& RHS)const {
                return My_t(*this) /= RHS;
            }

            ///Makes this the set-difference of this and other
            const My_t operator-=(const My_t& RHS) {
                std::set<size_t> Temp(std::move(this->Elems_));
                this->Elems_ = std::set<size_t>();
                std::set_difference(Temp.begin(), Temp.end(),
                        RHS.Elems_.begin(), RHS.Elems_.end(),
                        std::inserter<std::set < size_t >> (
                        this->Elems_, this->Elems_.begin()));
                return *this;
            }

            ///Returns the set-difference of this and other
            My_t operator-(const My_t& RHS)const {
                return My_t(*this) -= RHS;
            }

            ///Returns the complement of this
            My_t Complement()const {
                My_t Temp;
                Temp.Universe_ = Universe_;
                for (const T& EI : *Universe_) {
                    if (!this->Contains(EI))Temp << EI;
                }
                return Temp;
            }

            virtual std::string ToString()const {
                std::stringstream ss;
                for (const size_t& EI : this->Elems_)
                    ss << (*Universe_)[EI] << " ";
                return ss.str();
            }


            My_t Transform(std::function<T(const T&)> transformer)const
            {
                //! \todo better way to do this function?

                std::shared_ptr<Base_t> newuniverse(new Base_t);

                size_t i = 0;
                for(const auto & it : *Universe_)
                {
                    if(this->Elems_.count(i) > 0)
                        (*newuniverse) << transformer(it);
                    else
                        (*newuniverse) << it;
                    ++i;
                }

                // Copy all the other data, then replace the universe
                My_t ret(*this);
                ret.Universe_ = newuniverse;
                return ret;
            }


            void Select(std::function<bool(const T&)> selector)
            {
                std::set<size_t> newelems;
                for(const auto & idx : this->Elems_)
                {
                    const auto & el = (*Universe_)[idx];
                    if(selector(el))
                        newelems.insert(idx);
                }
                this->Elems_ = newelems; 
            }

            ///@{
            ///Basic forwarding of remaining functions and types

            typedef ConstSetItr<T, U> const_iterator;

            
            const_iterator begin() const {
                return const_iterator(Universe_->MakeIterator(this->Elems_.begin()));
            }

            const_iterator end() const { 
                return const_iterator(Universe_->MakeIterator(this->Elems_.end()));
            }


            ///@}
        };



        // Free function to help transform a class derived from MathSet
        template<typename T>
        T TransformMathSet(const T & mset,
                           std::function<typename T::value_type(const typename T::value_type)> transformer)
        { 
            typedef MathSet<typename T::value_type, typename T::store_type> Base_t;
            T ret(mset); // copies all members of mset, but shallow copies universe

            // Deep copies universe, etc
            //! \todo Set only the MathSet part? UGLY. Consider replacing with a function
            static_cast<Base_t &>(ret).operator=(mset.Transform(transformer));
            return ret;
        }

        // Free function to help transform a class derived from MathSet
        template<typename T>
        T PartitionMathSet(const T & mset,
                           std::function<bool(const typename T::value_type)> selector)
        { 
            typedef MathSet<typename T::value_type, typename T::store_type> Base_t;
            T ret(mset); // copies all members of mset, but shallow copies universe
            ret.Select(selector);
            return ret;
        }



    }
}//End namespaces

#endif /* BPMODULE_GUARD_MATHSET_HPP_ */

