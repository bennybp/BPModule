/*! \file
 *
 * \brief The definition of the interface for a module that meets the definition
 * of the concept of a method (i.e. computes energy derivatives)
 * 
 * \author Ryan Richard
 */ 


#ifndef PULSAR_GUARD_MODULEBASE__ENERGYMETHOD_HPP_
#define PULSAR_GUARD_MODULEBASE__ENERGYMETHOD_HPP_

#include <vector>
#include "pulsar/modulebase/ModuleBase.hpp"


namespace pulsar{
namespace modulebase {

/*! \brief This is the base class for modules that can compute derivatives of
 *  the electronic energy with respect to nuclear coordinates.
 * 
 *  We insist on a common name for the call to the energy, Deriv, so that we can
 *  write generic routines for combining the results without if/then logic.
 * 
 *  This base class will take care of all necessary finite difference 
 *  computations as determined by the MAX_DERIV keyword.  Obviously, we can not
 *  finite difference the energy for you....
 *
 */
class EnergyMethod : public ModuleBase
{   
    public:
        typedef EnergyMethod BaseType;
        typedef std::pair<datastore::Wavefunction, double> EnergyReturnType;
        typedef std::pair<datastore::Wavefunction, std::vector<double>> DerivReturnType;
        
        EnergyMethod(ID_t id): ModuleBase(id,"EnergyMethod"){ }
        
        ///The call users use, tries to use available analytic derivatives
        DerivReturnType Deriv(size_t Order, const datastore::Wavefunction & wfn)
        {
            return ModuleBase::CallFunction(&EnergyMethod::SplitDeriv_, Order, wfn);
        }

        ///Returns the maximum Order for which analytic derivative are coded
        ///reads it from option MAX_DERIV
        size_t MaxDeriv() const
        {
            return ModuleBase::CallFunction(&EnergyMethod::MaxDeriv_);
        }
        
         

        /** \brief Computes your Order-th derivative via Finite difference
         * 
         *   I originally made this protected, but then realized people
         *   may want to obtain non-analytic derivatives for an Order
         *   for which they exist.  This function will continue recursively
         *   calling Deriv until analytic derivatives are available.  Hence,
         *   setting MAX_DERIV controls the level of nesting
         */ 
        DerivReturnType FiniteDifference(size_t Order, const datastore::Wavefunction & wfn)
        {
            return ModuleBase::CallFunction(&EnergyMethod::FiniteDifference_, Order, wfn);
        }

        ///@{
        ///Convenience functions
        ///Returns the energy of this method
        EnergyReturnType Energy(const datastore::Wavefunction & wfn){
            DerivReturnType ret = Deriv(0,wfn);
            return {ret.first, ret.second[0]};
        }

        ///Returns the nuclear gradient of this method
        DerivReturnType Gradient(const datastore::Wavefunction & wfn){return Deriv(1,wfn);}

        ///Returns the nuclear Hessian of this method
        DerivReturnType Hessian(const datastore::Wavefunction & wfn){return Deriv(2,wfn);}
        ///@}


        //////////////////////////////////////////////////
        // To be implemented by derived classes
        //////////////////////////////////////////////////
        virtual DerivReturnType Deriv_(size_t Order, const datastore::Wavefunction & wfn) = 0;
        


    private:
        DerivReturnType SplitDeriv_(size_t Order, const datastore::Wavefunction & wfn)
        {
            if(Order > MaxDeriv())
                return FiniteDifference(Order, wfn);
            else
               return ModuleBase::CallFunction(&EnergyMethod::Deriv_,Order, wfn);
        }

        size_t MaxDeriv_(void) const;

        DerivReturnType FiniteDifference_(size_t Order, const datastore::Wavefunction & wfn);
};

///Specialization so that Python calls the right virtual function
class EnergyMethod_Py : public EnergyMethod{
    public:
        using EnergyMethod::EnergyMethod;
        MODULEBASE_FORWARD_PROTECTED_TO_PY
                
        virtual DerivReturnType Deriv_(size_t Order,const datastore::Wavefunction & wfn){
            return CallPyOverride<DerivReturnType>("Deriv_",Order,wfn);
        }    
};

} // close namespace modulebase
} // close namespace pulsar

#endif /* PULSAR_GUARD_MODULEBASE__ENERGYMETHOD_HPP_ */
