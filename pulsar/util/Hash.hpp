/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/** \file Hash.hpp
 *  \brief Contains hash functions to allow std objects to be used in unordered
 *         containers.  If at some point C++ defines them we should use those
 *         instead
 *  \author Ryan M. Richard
 *  \version 1.0
 *  \date May 18, 2016
 */

#ifndef PULSAR_GUARD_HASH_HPP
#define PULSAR_GUARD_HASH_HPP

#include <utility>


namespace std
{

template<typename LHS,typename RHS>
struct hash<std::pair<LHS,RHS>>
{
    typedef std::pair<LHS,RHS> argument_type;
    typedef std::size_t result_type;
    result_type operator()(argument_type const& p)const{
        result_type const h1 (std::hash<LHS>()(p.first));
        result_type const h2 (std::hash<RHS>()(p.second));
        return h1 ^ (h2<< 1);//Stolen from cppreference.com
    }
};

}//end namespace std


#endif /* PULSAR_GHUARD_HASH_HPP */

