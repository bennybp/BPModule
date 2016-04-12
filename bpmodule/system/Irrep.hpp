/*! \file
 *
 * \brief Irreducible representations list
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef BPMODULE_GUARD_SYSTEM__IRREP_HPP_
#define BPMODULE_GUARD_SYSTEM__IRREP_HPP_


namespace bpmodule{
namespace system{

//! \todo Add the rest
//! \todo ToString, etc
enum class Irrep {
    A1g, A1u, A1,
    A2g, A2u, A1,
    B1g, B1u, B1,
    B2g, B2u, B2,
    E1g, E1u, E1,
    E2g, E2u, E2;
};



} // close namespace system
} // close namespace bpmodule

#endif

