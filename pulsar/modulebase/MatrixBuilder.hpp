/*! \file
 *
 * \brief Caching implementation of one-electron integrals (header)
 * \author Ben Pritchard (ben@bennyp.org)
 */


#pragma once

#include "pulsar/modulebase/ModuleBase.hpp"
#include "pulsar/system/BasisSet.hpp"

namespace pulsar{

/*! \brief A module base type for building matrices from a wavefunction, and
 *  two basis sets (one for the rows and one for the columns).
 */
class MatrixBuilder : public ModuleBase
{
    public:
        typedef MatrixBuilder BaseType;
        typedef std::vector<std::shared_ptr<const MatrixDImpl>> ReturnType;

        MatrixBuilder(ID_t id)
            : ModuleBase(id, "OneElectronMatrix")
        { }


        /*! \brief calculate the integral matrix
         *
         * \param [in] shell1 Shell index on the first center
         * \param [in] shell2 Shell index on the second center
         * \param [in] outbuffer Where to place the completed integrals
         * \param [in] bufsize Size of \p outbuffer (as the number of doubles)
         * \return Number of integrals calculated
         */
        ReturnType calculate(const std::string & key,
                             unsigned int deriv, const Wavefunction & wfn,
                             const BasisSet & bs1, const BasisSet & bs2)
        {
            return ModuleBase::call_function(&MatrixBuilder::calculate_,
                                                key, deriv, wfn, bs1, bs2);
        }


        /////////////////////////////////////////
        // To be implemented by derived classes
        /////////////////////////////////////////

        //! \copydoc calculate
        virtual ReturnType calculate_(const std::string & key,
                                      unsigned int deriv, const Wavefunction & wfn,
                                      const BasisSet & bs1, const BasisSet & bs2) = 0;

};


class MatrixBuilder_Py : public MatrixBuilder
{
    public:
        using MatrixBuilder::MatrixBuilder;

        MODULEBASE_FORWARD_PROTECTED_TO_PY

        virtual ReturnType calculate_(const std::string & key,
                                      unsigned int deriv, const Wavefunction & wfn,
                                      const BasisSet & bs1, const BasisSet & bs2)
        {
            return call_py_override<ReturnType>(this, "calculate_", key, deriv, wfn, bs1, bs2);
        }
};

} // close namespace pulsar
