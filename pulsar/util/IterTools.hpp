/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/** \file IterTools.hpp
 *  \brief Helper classes for iterating
 *  \author Ryan M. Richard
 *  \version 1.0
 *  \date May 23, 2016
 */

#ifndef PULSAR_GUARD_ITERTOOLS_HPP
#define PULSAR_GUARD_ITERTOOLS_HPP

namespace pulsar {
namespace util{

/** \brief An object that evaluates to an integer range for use in range based
 *         for loops
 * 
 *  The design of this class is stolen from StackOverflow.  Usage is like:
 *  \code
 *  for(size_t i : Range<0,5>)
 *      std::cout<<i<<std::endl;
 *  \endcode
 *  Output should be:
 *  \verbatim
 *  0
 *  1
 *  2
 *  3
 *  4
 *  \endverbatim
 *  
 *  Iteration range is [Begin,End)
 */
template<size_t Begin,size_t End>
struct Range{
    struct iterator{
        size_t value_;
        iterator(size_t v):value_(v){}
        size_t operator*()const{return value_;}
        bool operator==(const iterator& other)const{
            return value_==other.value_;
        }
        bool operator!=(const iterator& other)const{
            return !((*this)==other);
        }
        iterator operator++(){++value_;return *this;}
    };
    iterator begin()const{return iterator(Begin);}
    iterator end()const{return iterator(End);}
};

}}//End namespace pulsar
#endif /* PULSAR_GHUARD_ITERTOOLS_HPP */

