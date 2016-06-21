/*! \file
 *
 * \brief Irreducible representations list
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_MATH__IRREP_HPP_
#define PULSAR_GUARD_MATH__IRREP_HPP_


namespace pulsar{
namespace math{

//! \todo Add the rest
//! \todo to_string, etc
enum class Irrep {
  A,
  A1, A1g, A1u,
  A2, A2g, A2u,
  B1, B1g, B1u,
  B2, B2g, B2u,
  E1, E1g, E1u,
  E2, E2g, E2u,
};



} // close namespace math
} // close namespace pulsar

#endif

