/* 
 * File:   Binomial.hpp
 *
 * Created on March 17, 2016, 6:42 PM
 */

#ifndef PULSAR_GUARD_MATH__BINOMIAL_HPP_
#define PULSAR_GUARD_MATH__BINOMIAL_HPP_

#include <cstdlib>

namespace pulsar{
namespace math{

/** \brief Computes the beta function of a and b
 * 
 *  By definition, in terms of the gamma function, the beta function is:
 *    
 *  \f[
 *    \beta(a,b)=\frac{\Gamma(a)\Gamma(b)}{\Gamma(a+b)}
 * \f]
 * 
 * or written in terms of logarithms:
 * \f[
 *   ln\beta(a,b)=ln\Gamma(a)+ln\Gamma(b)-ln\Gamma(a+b)
 * \f]
 * 
 * the log of the Gamma function is in the C++ math library.  Every time a and
 * b increase by an order of magnitude we loose one digit of precision though.
 * For modest a and b this should be acceptable, but for a and b on the order
 * of tens of thousands one needs to start being careful.
 * 
 * Boost's implementation does it in terms of a Lanczos approximation, which
 * when carried out with the appropriate number of terms, essentially produces
 * the exact value for anything that can fit in a double.
 * 
 *\todo Write in terms of Lanczos approximations for better precision
 */
double beta(double a,double b);


/** \brief Computes n choose k
 * 
 *  At the moment this function handles up to 170 choose k with factorial
 *  lookups.  To go beyond that requires the beta function (which requires the
 *  gamma) function.  I have used the log gamma function to implement the beta
 *  function, so for n up to about 10,000 we should be fine.  See note in
 *  beta function for going further.
 */ 
double binomial_coefficient(size_t n, size_t k);



}}//End namespaces
#endif /* BINOMIAL_HPP */

