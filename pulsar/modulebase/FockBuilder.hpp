/*! \file
 *
 * \brief A module that builds a Fock matrix
 */


#ifndef PULSAR_GUARD_MODULEBASE__FOCKBUILDER_HPP_
#define PULSAR_GUARD_MODULEBASE__FOCKBUILDER_HPP_

#include "pulsar/modulebase/ModuleBase.hpp"


namespace pulsar{

/*! \brief One-electron integral implementation
 *
 */
class FockBuilder : public ModuleBase
{
    public:
        typedef FockBuilder BaseType;

        FockBuilder(ID_t id)
            : ModuleBase(id, "FockBuilder")
        { }


        /*! \brief initialize the fock matrix computation
         *
         * \param [in] deriv Derivative to calculate
         * \param [in] wfn Wavefunction to use to calculate integrals
         * \param [in] bs1 Basis set to use
         */
        void initialize(unsigned int deriv,
                        const Wavefunction & wfn,
                        const BasisSet & bs)
        {
            return ModuleBase::call_function(&FockBuilder::initialize_, deriv, wfn, bs);
        }



        /*! \brief calculate an element of the fock matrix
         *
         * \param [in] shell1 Shell index on the first center
         * \param [in] shell2 Shell index on the second center
         * \param [in] outbuffer Where to place the completed integrals
         * \param [in] bufsize Size of \p outbuffer (as the number of doubles)
         * \return Number of integrals calculated
         */
        IrrepSpinMatrixD calculate(const Wavefunction & wfn)
        {
            return ModuleBase::call_function(&FockBuilder::calculate_, wfn);
        }



        /////////////////////////////////////////
        // To be implemented by derived classes
        /////////////////////////////////////////
        //! \copydoc initialize
        virtual void initialize_(unsigned int deriv,
                                 const Wavefunction & wfn,
                                 const BasisSet & bs) = 0;


        //! \copydoc calculate
        virtual IrrepSpinMatrixD calculate_(const Wavefunction & wfn) = 0;
};


class FockBuilder_Py : public FockBuilder
{
    public:
        using FockBuilder::FockBuilder;

        MODULEBASE_FORWARD_PROTECTED_TO_PY

        virtual void initialize_(unsigned int deriv,
                                 const Wavefunction & wfn,
                                 const BasisSet & bs)

        {
            return call_py_override<void>(this, "initialize_", deriv, wfn, bs);
        }


        virtual IrrepSpinMatrixD calculate_(const Wavefunction & wfn)
        {
            return call_py_override<IrrepSpinMatrixD>(this, "calculate_", wfn);
        }
};

} // close namespace pulsar

#endif
