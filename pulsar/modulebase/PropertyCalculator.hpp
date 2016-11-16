/*! \file
 *
 * \brief Calculation of properties
 * \author Ben Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_MODULEBASE__PROPERTYCALCULATOR_HPP_
#define PULSAR_GUARD_MODULEBASE__PROPERTYCALCULATOR_HPP_

#include "pulsar/modulebase/ModuleBase.hpp"


namespace pulsar{

/*! \brief One-electron integral implementation
 *
 */
class PropertyCalculator : public ModuleBase
{
    public:
        typedef PropertyCalculator BaseType;

        PropertyCalculator(ID_t id)
            : ModuleBase(id, "PropertyCalculator")
        { }


        /*! \brief calculate a property
         *
         */
        std::vector<double> calculate(unsigned int deriv,
                                      const Wavefunction & wfn,
                                      const BasisSet & bs1,
                                      const BasisSet & bs2)
        {
            return ModuleBase::call_function(&PropertyCalculator::calculate_,
                                                deriv, wfn, bs1, bs2);
        }


        /////////////////////////////////////////
        // To be implemented by derived classes
        /////////////////////////////////////////
        //! \copydoc calculate
        virtual std::vector<double> calculate_(unsigned int deriv,
                                               const Wavefunction & wfn,
                                               const BasisSet & bs1,
                                               const BasisSet & bs2) = 0;
};


class PropertyCalculator_Py : public PropertyCalculator
{
    public:
        using PropertyCalculator::PropertyCalculator;

        MODULEBASE_FORWARD_PROTECTED_TO_PY

        virtual std::vector<double> calculate_(unsigned int deriv,
                                               const Wavefunction & wfn,
                                               const BasisSet & bs1,
                                               const BasisSet & bs2)
        {
            return call_py_override<std::vector<double>>(this, "calculate_", wfn, deriv, bs1, bs2);
        }
};

} // close namespace pulsar

#endif
