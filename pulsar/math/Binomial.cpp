/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <cmath>
#include <ctgmath>
#include <math.h>       /* math_errhandling */
#include <errno.h>      /* errno, EDOM */
#include <fenv.h>       /* feclearexcept, fetestexcept, FE_ALL_EXCEPT, FE_INVALID */
#include "pulsar/math/Binomial.hpp"
#include "pulsar/math/Factorial_LUT.hpp"
#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/pragma.h"


PRAGMA_WARNING_PUSH
PRAGMA_WARNING_IGNORE_UNRECOGNIZED_PRAGMA  
//No idea what this does, but it's in the C++ examples on cplusplus.com
//Intel at least doesn't seem to like it
#pragma STDC FENV_ACCESS on
PRAGMA_WARNING_POP
using pulsar::exception::GeneralException;

namespace pulsar{
    namespace math{

double beta(double a,double b){
    errno=0;
    if(math_errhandling & MATH_ERREXCEPT)feclearexcept(FE_ALL_EXCEPT);
    double result=lgamma(a);
    result+=lgamma(b);
    result-=lgamma(a+b);
    
    //Check for errors in gamma function access:
    if(math_errhandling &MATH_ERREXCEPT){
        if(fetestexcept(FE_OVERFLOW))
            throw GeneralException("Beta function has overflowed");
        else if(fetestexcept(FE_DIVBYZERO))
            throw GeneralException("Beta function is at a pole");
    }
    else if(math_errhandling & MATH_ERRNO)
        if(errno==EDOM)
            throw GeneralException("Beta function error");
    
    //Result is good
    return exp(result);
}        
     
double binomial_coefficient(size_t n, size_t k){
    if(k>n)return 0.0;
    if((k==0)||(k==n))return 1.0;
    if((k==1)||(k==n-1))return (double)n;
    double result;
    if(n<lut::FAC_DOUBLE_LUT_MAX_){
        result=lut::fac_double_lut_[n];
        result/=lut::fac_double_lut_[n-k];
        result/=lut::fac_double_lut_[k];
    }
    else{
        if(k<n-k)
            result=k*beta((double)k,(double)(n-k+1));
        else
            result=(n-k)*beta((double)(k+1),double(n-k));
    }    
    return result;
}

}}//End namespaces

