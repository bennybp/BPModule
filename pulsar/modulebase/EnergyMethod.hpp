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

typedef std::pair<Wavefunction, double> EnergyReturnType;
typedef std::pair<Wavefunction, std::vector<double>> DerivReturnType;



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

        EnergyMethod(ID_t id): ModuleBase(id,"EnergyMethod"){ }

        ///The call users use, tries to use available analytic derivatives
        DerivReturnType deriv(size_t Order, const Wavefunction & wfn)
        {
            return ModuleBase::call_function(&EnergyMethod::split_deriv_, Order, wfn);
        }

        ///Returns the maximum Order for which analytic derivative are coded
        ///reads it from option MAX_DERIV
        size_t max_deriv() const
        {
            return ModuleBase::call_function(&EnergyMethod::max_deriv_);
        }



        /** \brief Computes your Order-th derivative via Finite difference
         *
         *   I originally made this protected, but then realized people
         *   may want to obtain non-analytic derivatives for an Order
         *   for which they exist.  This function will continue recursively
         *   calling Deriv until analytic derivatives are available.  Hence,
         *   setting MAX_DERIV controls the level of nesting
         */
        DerivReturnType finite_difference(size_t Order, const Wavefunction & wfn)
        {
            return ModuleBase::call_function(&EnergyMethod::finite_difference_, Order, wfn);
        }

        ///@{
        ///Convenience functions
        ///Returns the energy of this method
        EnergyReturnType energy(const Wavefunction & wfn){
            DerivReturnType ret = deriv(0,wfn);
            return {ret.first, ret.second[0]};
        }

        ///Returns the nuclear gradient of this method
        DerivReturnType gradient(const Wavefunction & wfn){return deriv(1,wfn);}

        ///Returns the nuclear Hessian of this method
        DerivReturnType hessian(const Wavefunction & wfn){return deriv(2,wfn);}
        ///@}


        //////////////////////////////////////////////////
        // To be implemented by derived classes
        //////////////////////////////////////////////////
        virtual DerivReturnType deriv_(size_t Order, const Wavefunction & wfn) = 0;



    private:
        DerivReturnType split_deriv_(size_t Order, const Wavefunction & wfn)
        {
            if(Order > max_deriv())
                return finite_difference(Order, wfn);
            else
               return ModuleBase::call_function(&EnergyMethod::deriv_,Order, wfn);
        }

        size_t max_deriv_(void) const;

        DerivReturnType finite_difference_(size_t Order, const Wavefunction & wfn);
};

///Specialization so that Python calls the right virtual function
class EnergyMethod_Py : public EnergyMethod{
    public:
        using EnergyMethod::EnergyMethod;
        MODULEBASE_FORWARD_PROTECTED_TO_PY

        virtual DerivReturnType deriv_(size_t Order,const Wavefunction & wfn){
            return call_py_override<DerivReturnType>(this, "deriv_",Order,wfn);
        }
};

} // close namespace pulsar

#endif /* PULSAR_GUARD_MODULEBASE__ENERGYMETHOD_HPP_ */
