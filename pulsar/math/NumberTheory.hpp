/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/** \file NumberTheory.hpp
 *  \author: Ryan M. Richard
 *  \version 1.0a
 *  \date April 14, 2016
 *   \brief Contains free functions for number-theory related operations such
 *          as finding primes, factoring numbers, etc.
 */

#ifndef PULSAR_GUARD_MATH__NUMBERTHEORY_HPP_
#define PULSAR_GUARD_MATH__NUMBERTHEORY_HPP_
#include<set>
#include<algorithm>
#include<utility>
#include "pulsar/math/Checking.hpp"
#include "pulsar/math/Cast.hpp"
#include "pulsar/exception/PulsarException.hpp"

namespace pulsar{

///Computes and returns all factors of \p Num (including 1 and Num)
template<typename T>
std::set<T> factors(T Num){
    if(!is_integer(Num))
        throw pulsar::PulsarException("Integer required");
    std::set<T> f;
    T max=(T)floor(sqrt(numeric_cast<double>(Num)));
    for(T i=1;i<=max;++i)
        if(Num%i==0){
            f.insert(i);
            f.insert(Num/i);
        }
    return f;
}

///Returns true if \p Num is prime
template<typename T>
bool is_prime(T Num){
    std::set<T> facs=factors(Num);
    return facs.size()==2;
}



///Variadic template recursion endpoint
template<typename T>
std::set<T> common_factors(T N){return factors(N);}

///Returns a set of factors common to n numbers
template<typename T,typename ...Args>
std::set<T> common_factors(T N,Args ...args){
    std::set<T> Int=common_factors(args...);
    if(Int.size()==0)return Int;
    std::set<T> CommonFs,MyFs=factors(N);
    std::set_intersection(Int.begin(),Int.end(),MyFs.begin(),MyFs.end(),
                          std::inserter(CommonFs,CommonFs.begin()));
    return CommonFs;
}

///Computes the largest common factor of a series of numbers
template<typename T,typename ...Args>
T largest_common_factor(T N1,Args ...args){
    std::set<T> LCFs=common_factors(N1,args...);
    return *LCFs.rbegin();
}

///Returns true if the numbers are relatively prime (no common factors)
template<typename ...Args>
bool relatively_prime(Args...args){
    return common_factors(args...).size()==1;
}

///Reduces the fraction \p n over \p d, returns reduced pair (n,d)
template<typename T>
std::pair<T,T> reduce(T n,T d){
    T LF=largest_common_factor(n,d);
    if(LF==0)return std::make_pair(n,d);
    return std::make_pair(n/LF,d/LF);
}

}
#endif /* NUMBERTHEORY_HPP */

