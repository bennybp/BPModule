/*! \file
 *
 * \brief Irreducible representations list
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_MATH__IRREP_HPP_
#define PULSAR_GUARD_MATH__IRREP_HPP_

#include<map>

namespace pulsar{
namespace math{

///A list of irreps found in Albein point groups
enum class Irrep {
  A,Ag,Au,Ap,App,
  B,Bg,Bu,
  A1, A1g, A1u,
  A2, A2g, A2u,
  B1, B1g, B1u,
  B2, B2g, B2u,
  E1, E1g, E1u,
  E2, E2g, E2u,
};

///A map from irrep to its string equivalent
extern const std::map<Irrep,std::string> irrep_to_string;


///To ensure we do alpha/beta consistent
enum Spin {//purposely use old-school enums to allow falling back to ints
    alpha=1,beta=-1
};

///A map from a spin to its string equivalent
extern const std::map<Spin,std::string> spin_to_string;

} // close namespace math
} // close namespace pulsar

#endif

