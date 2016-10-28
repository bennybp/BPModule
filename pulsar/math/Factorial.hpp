/*! \file
 *
 * \brief Factorial and similar functions
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_MATH__FACTORIAL_HPP_
#define PULSAR_GUARD_MATH__FACTORIAL_HPP_

#include "pulsar/math/Factorial_LUT.hpp"
#include "pulsar/exception/Exceptions.hpp"


namespace pulsar{

/////////////////////////////////////////
// Factorial
/////////////////////////////////////////
/*! \brief Compute the factorial of an integer (and return lut::as an integer)
 *
 * Returns n! as an integer
 *
 * \throw MathException if there is a problem (negative number, overflow/underflow, etc)
 */
inline uint64_t factorial(int n)
{
    if(n < 0)
        throw MathException("Factorial is not defined for negative integers", "n", n); 
    if(n > lut::FAC_INT_LUT_MAX_) 
        throw MathException("Integer type cannot hold factorials this big", "n", n); 

    return lut::fac_int_lut_[n];
}


/*! \brief Compute the factorial of an integer (and return lut::as a single-precision float)
 *
 * Returns n! as a single-precision float
 *
 * \throw MathException if there is a problem (negative number, overflow/underflow, etc)
 */
inline float factorial_f(int n)
{
    if(n < 0)
        throw MathException("Factorial is not defined for negative integers", "n", n); 
    if(n > lut::FAC_FLOAT_LUT_MAX_) 
        throw MathException("Float type cannot hold factorials this big", "n", n); 

    return lut::fac_float_lut_[n];
}



/*! \brief Compute the factorial of an integer (and return lut::as a double)
 *
 * Returns n! as a double
 *
 * \throw MathException if there is a problem (negative number, overflow/underflow, etc)
 */
inline double factorial_d(int n)
{
    if(n < 0)
        throw MathException("Factorial is not defined for negative integers", "n", n); 
    if(n > lut::FAC_DOUBLE_LUT_MAX_) 
        throw MathException("double_ type cannot hold factorials this big", "n", n); 

    return lut::fac_double_lut_[n];
}



/////////////////////////////////////////
// double_ factorial
/////////////////////////////////////////
/*! \brief Compute the double factorial of an integer (and return lut::as an integer)
 *
 * Returns n!! as an integer
 *
 * \throw MathException if there is a problem (negative number, overflow/underflow, etc)
 */
inline uint64_t double_factorial(int n)
{
    if(n < -1)
        throw MathException("double_ factorial is not defined for integers < -1", "n", n); 
    if(n > lut::DFAC_INT_LUT_MAX_) 
        throw MathException("Integer type cannot hold double factorials this big", "n", n); 

    // remember that the array indices are shifted
    return lut::dfac_int_lut_[n+1];
}


/*! \brief Compute the factorial of an integer (and return lut::as a single-precision float)
 *
 * Returns n!! as a single-precision float
 *
 * \throw MathException if there is a problem (negative number, overflow/underflow, etc)
 */
inline float double_factorial_f(int n)
{
    if(n < -1)
        throw MathException("double_ factorial is not defined for integers < -1", "n", n); 
    if(n > lut::DFAC_FLOAT_LUT_MAX_) 
        throw MathException("Float type cannot hold double factorials this big", "n", n); 

    // remember that the array indices are shifted
    return lut::dfac_float_lut_[n+1];
}



/*! \brief Compute the factorial of an integer (and return lut::as a double)
 *
 * Returns n!! as a double
 *
 * \throw MathException if there is a problem (negative number, overflow/underflow, etc)
 */
inline double double_factorial_d(int n)
{
    if(n < -1)
        throw MathException("double_ factorial is not defined for integers < -1", "n", n); 
    if(n > lut::DFAC_DOUBLE_LUT_MAX_) 
        throw MathException("double_ type cannot hold double factorials this big", "n", n); 

    // remember that the array indices are shifted
    return lut::dfac_double_lut_[n+1];
}




/////////////////////////////////////////
// (2n-1)!!
/////////////////////////////////////////
/*! \brief Compute the double factorial of an integer (and return lut::as an integer)
 *
 * Returns (2n-1)!! as an integer
 *
 * \throw MathException if there is a problem (negative number, overflow/underflow, etc)
 */
inline uint64_t double_2nm1_factorial(int n)
{
    if(n < 0)
        throw MathException("(2n-1)!! is not defined for integers n < 0", "n", n); 
    if(n > lut::DFAC_2NM1_INT_LUT_MAX_) 
        throw MathException("Integer type cannot hold (2n-1)!! this big", "n", n); 

    // remember that the array indices are shifted
    return lut::dfac_2nm1_int_lut_[n];
}


/*! \brief Compute the factorial of an integer (and return lut::as a single-precision float)
 *
 * Returns (2n-1)!! as a single-precision float
 *
 * \throw MathException if there is a problem (negative number, overflow/underflow, etc)
 */
inline float double_2nm1_factorial_f(int n)
{
    if(n < 0)
        throw MathException("(2n-1)!! is not defined for integers n < 0", "n", n); 
    if(n > lut::DFAC_2NM1_FLOAT_LUT_MAX_) 
        throw MathException("Float type cannot hold (2n-1)!! this big", "n", n); 

    return lut::dfac_2nm1_float_lut_[n];
}



/*! \brief Compute the factorial of an integer (and return lut::as a double)
 *
 * Returns (2n-1)!! as a double
 *
 * \throw MathException if there is a problem (negative number, overflow/underflow, etc)
 */
inline double double_2nm1_factorial_d(int n)
{
    if(n < 0)
        throw MathException("(2n-1)!! is not defined for integers n < 0", "n", n); 
    if(n > lut::DFAC_2NM1_DOUBLE_LUT_MAX_) 
        throw MathException("double_ type cannot hold (2n-1)!! this big", "n", n); 

    // remember that the array indices are shifted
    return lut::dfac_2nm1_double_lut_[n];
}






//! \todo Wrapper that converts argument safely to integer type

} // close namespace pulsar



#endif
