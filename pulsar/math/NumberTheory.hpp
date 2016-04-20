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
#include "pulsar/exception/Exceptions.hpp"

namespace pulsar{
namespace math{

///Computes and returns all factors of \p Num
template<typename T>
std::set<T> Factors(T Num){
    if(!IsInteger(Num))
        throw pulsar::exception::GeneralException("Integer required");
    std::set<T> f;
    for(T i=1;i<=(T)floor(sqrt(numeric_cast<double>(Num)));++i)
        if(Num%i==0){
            f.insert(i);
            f.insert(Num/i);
        }
    return f;
}

///Variadic template recursion endpoint
template<typename T>
std::set<T> CommonFactors(T N){return Factors(N);}

///Returns a set of factors common to n numbers
template<typename T,typename ...Args>
std::set<T> CommonFactors(T N,Args ...args){
    std::set<T> Int=CommonFactors(args...);
    if(Int.size()==0)return Int;
    std::set<T> CommonFs,MyFs=Factors(N);
    std::set_intersection(Int.begin(),Int.end(),MyFs.begin(),MyFs.end(),
                          std::inserter(CommonFs,CommonFs.begin()));
    return CommonFs;
}

///Computes the largest common factor of a series of numbers
template<typename T,typename ...Args>
T LargestCommonFactor(T N1,Args ...args){
    std::set<T> LCFs=CommonFactors(N1,args...);
    return *LCFs.rbegin();
}

///Reduces the fraction \p n over \p d, returns reduced pair (n,d)
template<typename T>
std::pair<T,T> Reduce(T n,T d){
    T LF=LargestCommonFactor(n,d);
    if(LF==0)return std::make_pair(n,d);
    return std::make_pair(n/LF,d/LF);
}

}}
#endif /* NUMBERTHEORY_HPP */

