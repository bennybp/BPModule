/*! \file
 *
 * \brief Caching implementation of one-electron integrals (header)
 * \author Ben Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_MODULEBASE__ONEELECTRONMATRIX_HPP_
#define PULSAR_GUARD_MODULEBASE__ONEELECTRONMATRIX_HPP_

#include "pulsar/modulebase/ModuleBase.hpp"
#include "pulsar/system/BasisSet.hpp"

namespace pulsar{
namespace modulebase {

/*! \brief One-electron integral implementation
 *
 */
class OneElectronMatrix : public ModuleBase
{
    public:
        typedef OneElectronMatrix BaseType;
        typedef std::vector<std::shared_ptr<const math::TensorImpl<2, double>>> ReturnType;

        OneElectronMatrix(ID_t id)
            : ModuleBase(id, "OneElectronMatrix")
        { }


        /*! \brief Calculate the integral matrix
         *
         * \param [in] shell1 Shell index on the first center
         * \param [in] shell2 Shell index on the second center
         * \param [in] outbuffer Where to place the completed integrals
         * \param [in] bufsize Size of \p outbuffer (as the number of doubles)
         * \return Number of integrals calculated
         */
        ReturnType Calculate(const std::string & key,
                             unsigned int deriv, const datastore::Wavefunction & wfn,
                             const system::BasisSet & bs1, const system::BasisSet & bs2)
        {
            return ModuleBase::FastCallFunction(&OneElectronMatrix::Calculate_,
                                                key, deriv, wfn, bs1, bs2);
        }


        /////////////////////////////////////////
        // To be implemented by derived classes
        /////////////////////////////////////////

        //! \copydoc Calculate
        virtual ReturnType Calculate_(const std::string & key,
                                      unsigned int deriv, const datastore::Wavefunction & wfn,
                                      const system::BasisSet & bs1, const system::BasisSet & bs2) = 0;

};


class OneElectronMatrix_Py : public OneElectronMatrix
{
    public:
        using OneElectronMatrix::OneElectronMatrix;

        MODULEBASE_FORWARD_PROTECTED_TO_PY

        virtual ReturnType Calculate_(const std::string & key,
                                      unsigned int deriv, const datastore::Wavefunction & wfn,
                                      const system::BasisSet & bs1, const system::BasisSet & bs2)
        {
            return CallPyOverride<ReturnType>("Calculate_", key, deriv, wfn, bs1, bs2);
        }
};

} // close namespace modulebase
} // close namespace pulsar

#endif
