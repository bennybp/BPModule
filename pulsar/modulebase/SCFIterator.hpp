/*! \file
 *
 * \brief An iteration in an SCF procedure
 */


#ifndef PULSAR_GUARD_MODULEBASE__SCFITERATOR_HPP_
#define PULSAR_GUARD_MODULEBASE__SCFITERATOR_HPP_

#include "pulsar/modulebase/ModuleBase.hpp"
#include "pulsar/math/IrrepSpinMatrix.hpp"


namespace pulsar{

class SCFIterator : public ModuleBase
{
    public:
        typedef SCFIterator BaseType;

        SCFIterator(ID_t id): ModuleBase(id, "SCFIterator") { }

        /*! \brief Form a new wavefunction based on the given wfn and fock matrix
         */
        Wavefunction next(const Wavefunction & wfn, const IrrepSpinMatrixD & fmat)
        {
            return ModuleBase::call_function(&SCFIterator::next_, wfn, fmat);
        }


        virtual Wavefunction next_(const Wavefunction & wfn, const IrrepSpinMatrixD & fmat) = 0;

};

class SCFIterator_Py : public SCFIterator{
    public:
        using SCFIterator::SCFIterator;
        MODULEBASE_FORWARD_PROTECTED_TO_PY

        virtual Wavefunction next_(const Wavefunction & wfn, const IrrepSpinMatrixD & fmat)
        {
            return call_py_override<Wavefunction>(this, "next_", wfn, fmat);
        }
};

} // close namespace pulsar

#endif
