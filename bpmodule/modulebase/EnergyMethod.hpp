/*! \file
 *
 * \brief The definition of the interface for a module that meets the definition
 * of the concept of a method (i.e. computes energy derivatives)
 * 
 * \author Ryan Richard
 */ 


#ifndef BPMODULE_GUARD_MODULEBASE__ENERGYMETHOD_HPP_
#define BPMODULE_GUARD_MODULEBASE__ENERGYMETHOD_HPP_

#include <vector>
#include "bpmodule/modulebase/ModuleBase.hpp"


namespace bpmodule {
namespace modulebase {

/*! \brief This is the base class for modules that can compute derivatives of
 *  the electronic energy with respect to nuclear coordinates.
 * 
 *  We insist on a common name for the call to the energy, Deriv, so that we can
 *  write generic routines for combining the results without if/then logic.
 * 
 *  As long as your derived class calls the base class's Deriv function for
 *  derivative orders higher than you have coded, this class will take care of
 *  all necessary finite difference computations.  Obviously, we can not
 *  finite difference the energy for you....
 *
 */
class EnergyMethod : public ModuleBase
{
    public:
        typedef EnergyMethod BaseType;
        
        EnergyMethod(ID_t id): ModuleBase(id,"EnergyMethod"){ }
        
        ///The call users use
        std::vector<double> Deriv(size_t Order){
            return ModuleBase::CallFunction(&EnergyMethod::Deriv_,Order);
        }
        
        /** This is the function you override.  For all orders that you haven't
         *  implemented you should defer to the base class's Deriv() function,
         *  e.g. for example if you only have gradients:
         * 
         * \code
         *  std::vector<double> MyClass::Deriv(size_t Order){
         *      if(Order>1)return Method::Deriv(Order);
         *      //Take care of orders with Order=0 and 1
         *      return result
         *   }
         * \endcode
         */ 
        virtual std::vector<double> Deriv_(size_t Order);

        ///@{
        ///Convenience functions
        ///Returns the energy of this method
        double Energy(){return Deriv(0)[0];}
        ///Returns the nuclear gradient of this method
        std::vector<double> Gradient(){return Deriv(1);}
        ///Returns the nuclear Hessian of this method
        std::vector<double> Hessian(){return Deriv(2);}
        ///@}
};

///Specialization so that Python calls the right virtual function
class EnergyMethod_Py : public EnergyMethod{
    public:
        using EnergyMethod::EnergyMethod;
        MODULEBASE_FORWARD_PROTECTED_TO_PY
                
        virtual std::vector<double> Deriv_(size_t Order){
            return CallPyOverride<std::vector<double>>("Deriv_",Order);
        }
    
};

} // close namespace modulebase
} // close namespace bpmodule

#endif /* BPMODULE_GUARD_MODULEBASE__ENERGYMETHOD_HPP_ */
