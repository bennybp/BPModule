/** 
 * @file WeightType.cpp
 * Original author:
 * @Author Ryan Richard (ryanmrichard1@gmail.com)
 * With other contributions from:
 *
 * Creation date:
 * @date Jan 15, 2016
 * @brief Instantiates the static members of the WeightType class
 */
#include "bpmodule/math/WeightType.hpp"

namespace bpmodule{
namespace math{

//std::set<const util::Enumeration<WeightType>* >
//   util::Enumeration<WeightType>::Enums_;

const WeightType WeightType::Weight("WEIGHT");
const WeightType WeightType::Z("ATOMIC NUMBER");
const WeightType WeightType::Mass("MASS");
const WeightType WeightType::Charge("CHARGE");
const WeightType WeightType::VDWR("VAN DER WAALS RADIUS");
const WeightType WeightType::CovR("COVALENT RADIUS");
}}//End namespaces
