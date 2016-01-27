/** 
 * @file WeightTypes.hpp
 * Original author:
 * @Author Ryan Richard (ryanmrichard1@gmail.com)
 * With other contributions from:
 *
 * Creation date:
 * @date Jan 15, 2016
 * @brief Definitions of the recognized weight type labels.
 */
#ifndef BPMODULE_MATH_WEIGHTTYPE_HPP_
#define BPMODULE_MATH_WEIGHTTYPE_HPP_

#include "bpmodule/util/Enumeration.hpp"

namespace bpmodule{
namespace math{


/** \brief The weight types we recognize
 *
 *  Admittedly the point and locus class are largely
 *  envisioned as being the underpinnings of the System
 *  class so the weights names are representative of those
 *  quantities.  Nevertheless, for more generic mathematical
 *  operations we provide the Weight enum.  If you add more
 *  enums don't forget to instantiate them in WeightType.cpp
 *
 *
class WeightType: public util::Enumeration<WeightType>{
   private:
      ///The base class's type
      typedef util::Enumeration<WeightType> Base_t;
      ///How we will instantiate each member
      WeightType(const std::string):Base_t(Name){}
   public:
      ///A generic weight
      static const WeightType Weight;
      ///The atomic number
      static const WeightType Z;
      ///The atomic mass
      static const WeightType Mass;
      ///The atomic charge
      static const WeightType Charge;
      ///The van Der Waals radius
      static const WeightType VDWR;
      ///The covalent radius
      static const WeightType CovR;

};*/


}}/* End namespaces */

#endif /* BPMODULE_MATH_WEIGHTTYPE_HPP_ */
