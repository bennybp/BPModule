#ifndef _GUARD_FACTORIAL_HPP_
#define _GUARD_FACTORIAL_HPP_

#include "bpmodule/math/Factorial_LUT.hpp"
#include "bpmodule/exception/MathException.hpp"


namespace bpmodule {
namespace math {



/////////////////////////////////////////
// Factorial
/////////////////////////////////////////
/*! \brief Compute the factorial of an integer (and return lut::as an integer)
 *
 * Returns n! as an integer
 *
 * \throw exception::MathException if there is a problem (negative number, overflow/underflow, etc)
 */
inline uint64_t Factorial(int n)
{
    if(n < 0)
        throw exception::MathException("Factorial is not defined for negative integers", "n", std::to_string(n)); 
    if(n > lut::FAC_INT_LUT_MAX_) 
        throw exception::MathException("Integer type cannot hold factorials this big", "n", std::to_string(n)); 

    return lut::fac_int_lut_[n];
}


/*! \brief Compute the factorial of an integer (and return lut::as a single-precision float)
 *
 * Returns n! as a single-precision float
 *
 * \throw exception::MathException if there is a problem (negative number, overflow/underflow, etc)
 */
inline float FactorialF(int n)
{
    if(n < 0)
        throw exception::MathException("Factorial is not defined for negative integers", "n", std::to_string(n)); 
    if(n > lut::FAC_FLOAT_LUT_MAX_) 
        throw exception::MathException("Float type cannot hold factorials this big", "n", std::to_string(n)); 

    return lut::fac_float_lut_[n];
}



/*! \brief Compute the factorial of an integer (and return lut::as a double)
 *
 * Returns n! as a double
 *
 * \throw exception::MathException if there is a problem (negative number, overflow/underflow, etc)
 */
inline double FactorialD(int n)
{
    if(n < 0)
        throw exception::MathException("Factorial is not defined for negative integers", "n", std::to_string(n)); 
    if(n > lut::FAC_DOUBLE_LUT_MAX_) 
        throw exception::MathException("Double type cannot hold factorials this big", "n", std::to_string(n)); 

    return lut::fac_double_lut_[n];
}



/////////////////////////////////////////
// Double factorial
/////////////////////////////////////////
/*! \brief Compute the double factorial of an integer (and return lut::as an integer)
 *
 * Returns n!! as an integer
 *
 * \throw exception::MathException if there is a problem (negative number, overflow/underflow, etc)
 */
inline uint64_t DoubleFactorial(int n)
{
    if(n < -1)
        throw exception::MathException("Double factorial is not defined for integers < -1", "n", std::to_string(n)); 
    if(n > lut::DFAC_INT_LUT_MAX_) 
        throw exception::MathException("Integer type cannot hold double factorials this big", "n", std::to_string(n)); 

    // remember that the array indices are shifted
    return lut::dfac_int_lut_[n+1];
}


/*! \brief Compute the factorial of an integer (and return lut::as a single-precision float)
 *
 * Returns n!! as a single-precision float
 *
 * \throw exception::MathException if there is a problem (negative number, overflow/underflow, etc)
 */
inline float DoubleFactorialF(int n)
{
    if(n < -1)
        throw exception::MathException("Double factorial is not defined for integers < -1", "n", std::to_string(n)); 
    if(n > lut::DFAC_FLOAT_LUT_MAX_) 
        throw exception::MathException("Float type cannot hold double factorials this big", "n", std::to_string(n)); 

    // remember that the array indices are shifted
    return lut::dfac_float_lut_[n+1];
}



/*! \brief Compute the factorial of an integer (and return lut::as a double)
 *
 * Returns n!! as a double
 *
 * \throw exception::MathException if there is a problem (negative number, overflow/underflow, etc)
 */
inline double DoubleFactorialD(int n)
{
    if(n < -1)
        throw exception::MathException("Double factorial is not defined for integers < -1", "n", std::to_string(n)); 
    if(n > lut::DFAC_DOUBLE_LUT_MAX_) 
        throw exception::MathException("Double type cannot hold double factorials this big", "n", std::to_string(n)); 

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
 * \throw exception::MathException if there is a problem (negative number, overflow/underflow, etc)
 */
inline uint64_t Double2nm1Factorial(int n)
{
    if(n < 0)
        throw exception::MathException("(2n-1)!! is not defined for integers n < 0", "n", std::to_string(n)); 
    if(n > lut::DFAC_2NM1_INT_LUT_MAX_) 
        throw exception::MathException("Integer type cannot hold (2n-1)!! this big", "n", std::to_string(n)); 

    // remember that the array indices are shifted
    return lut::dfac_2nm1_int_lut_[n];
}


/*! \brief Compute the factorial of an integer (and return lut::as a single-precision float)
 *
 * Returns (2n-1)!! as a single-precision float
 *
 * \throw exception::MathException if there is a problem (negative number, overflow/underflow, etc)
 */
inline float Double2nm1FactorialF(int n)
{
    if(n < 0)
        throw exception::MathException("(2n-1)!! is not defined for integers n < 0", "n", std::to_string(n)); 
    if(n > lut::DFAC_2NM1_FLOAT_LUT_MAX_) 
        throw exception::MathException("Float type cannot hold (2n-1)!! this big", "n", std::to_string(n)); 

    return lut::dfac_2nm1_float_lut_[n];
}



/*! \brief Compute the factorial of an integer (and return lut::as a double)
 *
 * Returns (2n-1)!! as a double
 *
 * \throw exception::MathException if there is a problem (negative number, overflow/underflow, etc)
 */
inline double Double2nm1FactorialD(int n)
{
    if(n < 0)
        throw exception::MathException("(2n-1)!! is not defined for integers n < 0", "n", std::to_string(n)); 
    if(n > lut::DFAC_2NM1_DOUBLE_LUT_MAX_) 
        throw exception::MathException("Double type cannot hold (2n-1)!! this big", "n", std::to_string(n)); 

    // remember that the array indices are shifted
    return lut::dfac_2nm1_double_lut_[n];
}






//! \todo Wrapper that converts argument safely to integer type




} // close namespace math
} // close namespace bpmodule



#endif
