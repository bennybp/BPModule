/*! \file
 *
 * \brief Calculation of properties
 * \author Ben Pritchard (ben@bennyp.org)
 */ 


#ifndef PULSAR_GUARD_MODULEBASE__PROPERTYCALCULATOR_HPP_
#define PULSAR_GUARD_MODULEBASE__PROPERTYCALCULATOR_HPP_

#include "pulsar/modulebase/ModuleBase.hpp"


namespace pulsar{
namespace modulebase {

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


        /*! \brief Calculate a property
         *
         */
        std::vector<double> Calculate(const datastore::Wavefunction & wfn,
                                      const std::string & bs1,
                                      const std::string & bs2)
        {
            return ModuleBase::FastCallFunction(&PropertyCalculator::Calculate_,
                                                wfn, bs1, bs2);
        }


        /////////////////////////////////////////
        // To be implemented by derived classes
        /////////////////////////////////////////
        //! \copydoc Calculate
        virtual std::vector<double> Calculate_(const datastore::Wavefunction & wfn,
                                      const std::string & bs1,
                                      const std::string & bs2) = 0;
};


class PropertyCalculator_Py : public PropertyCalculator
{
    public:
        using PropertyCalculator::PropertyCalculator;

        MODULEBASE_FORWARD_PROTECTED_TO_PY
    
        virtual std::vector<double> Calculate_(const datastore::Wavefunction & wfn,
                                      const std::string & bs1,
                                      const std::string & bs2)
        {
            return CallPyOverride<std::vector<double>>("Calculate_", wfn, bs1, bs2);
        }
};

} // close namespace modulebase
} // close namespace pulsar

#endif
