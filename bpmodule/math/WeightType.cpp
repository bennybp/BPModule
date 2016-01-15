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

const WeightType WeightType::Weight("Weight");
const WeightType WeightType::Z("Atomic Number");
const WeightType WeightType::Mass("Mass");
const WeightType WeightType::Charge("Charge");
const WeightType WeightType::VDWR("van Der Waals Radius");
const WeightType WeightType::CovR("Covalent Radius");

}}//End namespaces
