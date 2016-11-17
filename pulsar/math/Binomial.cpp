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
#include "pulsar/exception/PulsarException.hpp"
#include "pulsar/math/Cast.hpp"

using pulsar::PulsarException;

namespace pulsar{

double beta(double a,double b){
    errno=0;
    if(math_errhandling & MATH_ERREXCEPT)feclearexcept(FE_ALL_EXCEPT);
    double result=lgamma(a);
    result+=lgamma(b);
    result-=lgamma(a+b);
    
    //Check for errors in gamma function access:
    if(math_errhandling &MATH_ERREXCEPT){
        if(fetestexcept(FE_OVERFLOW))
            throw PulsarException("Beta function has overflowed");
        else if(fetestexcept(FE_DIVBYZERO))
            throw PulsarException("Beta function is at a pole");
    }
    else if(math_errhandling & MATH_ERRNO)
        if(errno==EDOM)
            throw PulsarException("Beta function error");
    
    //Result is good
    return exp(result);
}        
     
double binomial_coefficient(size_t n, size_t k){
    const double N=numeric_cast<double>(n);
    const double K=numeric_cast<double>(k);
    if(k>n)return 0.0;
    if((k==0)||(k==n))return 1.0;
    if((k==1)||(k==n-1))return N;
    double result;
    if(n<lut::FAC_DOUBLE_LUT_MAX_){
        result=lut::fac_double_lut_[n];
        result/=lut::fac_double_lut_[n-k];
        result/=lut::fac_double_lut_[k];
        return result;
    }
    return k<n-k ? K*beta(K,N-K+1.0) : (N-K)*beta(K+1,N-K);
}

}//End namespaces

