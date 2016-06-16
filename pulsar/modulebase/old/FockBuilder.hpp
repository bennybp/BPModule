/*! \file
 *
 * \brief A module that builds a Fock matrix
 */


#ifndef PULSAR_GUARD_MODULEBASE__FOCKBUILDER_HPP_
#define PULSAR_GUARD_MODULEBASE__FOCKBUILDER_HPP_

#include "pulsar/modulebase/ModuleBase.hpp"


namespace pulsar{
namespace modulebase {

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


        /*! \brief Initialize the fock matrix computation
         *
         * \param [in] deriv Derivative to calculate
         * \param [in] wfn Wavefunction to use to calculate integrals
         * \param [in] bs1 Basis set to use
         */
        void Initialize(unsigned int deriv,
                        const datastore::Wavefunction & wfn,
                        const system::BasisSet & bs)
        {
            return ModuleBase::CallFunction(&FockBuilder::Initialize_, deriv, wfn, bs);
        }



        /*! \brief Calculate an element of the fock matrix
         *
         * \param [in] shell1 Shell index on the first center
         * \param [in] shell2 Shell index on the second center
         * \param [in] outbuffer Where to place the completed integrals
         * \param [in] bufsize Size of \p outbuffer (as the number of doubles)
         * \return Number of integrals calculated
         */
        double Calculate(math::Irrep ir, int spin, uint64_t i, uint64_t j)
        {
            return ModuleBase::FastCallFunction(&FockBuilder::Calculate_,
                                                ir, spin, i, j);
        }


        /*! \brief Calculate multiple elements of the fock matrix
         *
         * \param [in] ibegin Starting row index
         * \param [in] iend Ending column index (not inclusive)
         * \param [in] jbegin Starting row index
         * \param [in] jend Ending column index (not inclusive)
         * \param [in] outbuffer Where to place the completed integrals
         * \param [in] bufsize Size of \p outbuffer (as the number of doubles)
         * \return Number of elements calculated
         */
        void CalculateMulti(math::Irrep ir, int spin,
                            uint64_t ibegin, uint64_t iend,
                            uint64_t jbegin, uint64_t jend,
                            double * outbuffer, size_t bufsize)
        {
            return ModuleBase::FastCallFunction(&FockBuilder::CalculateMulti_,
                                                ir, spin,
                                                ibegin, iend, jbegin, jend,
                                                outbuffer, bufsize);
        }


        /*! \brief Calculate multiple elements of the fock matrix (for use from python)
         *
         * \param [in] ibegin Starting row index
         * \param [in] iend Ending column index (not inclusive)
         * \param [in] jbegin Starting row index
         * \param [in] jend Ending column index (not inclusive)
         * \param [in] outbuffer Where to place the completed integrals
         * \return Number of elements calculated
         */
        void CalculateMultiPy(math::Irrep ir, int spin,
                              uint64_t ibegin, uint64_t iend,
                              uint64_t jbegin, uint64_t jend,
                              pybind11::buffer outbuffer)
        {
            auto ptrinfo = PythonBufferToPtr<double>(outbuffer, 1);

            return ModuleBase::FastCallFunction(&FockBuilder::CalculateMulti_,
                                                ir, spin,
                                                ibegin, iend, jbegin, jend,
                                                ptrinfo.first, ptrinfo.second[0]);
        }



        /////////////////////////////////////////
        // To be implemented by derived classes
        /////////////////////////////////////////
        //! \copydoc Initialize
        virtual void Initialize_(unsigned int deriv,
                                 const datastore::Wavefunction & wfn,
                                 const system::BasisSet & bs) = 0;


        //! \copydoc Calculate
        virtual double Calculate_(math::Irrep ir, int spin, uint64_t i, uint64_t j) = 0;

        //! \copydoc CalculateMulti
        virtual void CalculateMulti_(math::Irrep ir, int spin,
                                     uint64_t ibegin, uint64_t iend,
                                     uint64_t jbegin, uint64_t jend,
                                     double * outbuffer, size_t bufsize)
        {
            using namespace exception;

            if(ibegin > iend || jbegin > jend)
                throw GeneralException("Bad bounds for CalculateMulti",
                                       "ibegin", ibegin, "iend", iend,
                                       "jbegin", jbegin, "jend", jend);

            size_t totalsize = (iend-ibegin) * (jend-jbegin);

            if(bufsize < totalsize)
                throw GeneralException("Buffer not large enough in CalculateMulti",
                                       "bufsize", bufsize, "required", totalsize);


            //////////////////////////////////////////////////////////
            // default implementation - just loop over and do them all
            //////////////////////////////////////////////////////////
            size_t idx = 0;
            for(uint64_t i = ibegin; i = iend; ++i)
            for(uint64_t j = jbegin; j = jend; ++j)
                outbuffer[idx++] = Calculate_(ir, spin, i, j);
        }
};


class FockBuilder_Py : public FockBuilder
{
    public:
        using FockBuilder::FockBuilder;

        MODULEBASE_FORWARD_PROTECTED_TO_PY

        virtual void Initialize_(unsigned int deriv,
                                 const datastore::Wavefunction & wfn,
                                 const system::BasisSet & bs)

        {
            return CallPyOverride<void>("Initialize_", deriv, wfn, bs);
        }


        virtual double Calculate_(math::Irrep ir, int spin,
                                  uint64_t i, uint64_t j)
        {
            return CallPyOverride<double>("Calculate_", ir, spin, i, j);
        }


        virtual void CalculateMulti_(math::Irrep ir, int spin,
                                     uint64_t ibegin, uint64_t iend,
                                     uint64_t jbegin, uint64_t jend,
                                     double * outbuffer, size_t bufsize)
        {
            if(HasPyOverride("CalculateMulti_"))
            {
                //! \todo untested

                pybind11::buffer_info pybuf(outbuffer,
                                            sizeof(double),
                                            pybind11::format_descriptor<double>::value,
                                            1, { bufsize },
                                            { sizeof(double) });

                return CallPyOverride<void>("CalculateMulti_",
                                            ir, spin, ibegin, iend, jbegin, jend,
                                            pybuf, bufsize);
            }
            else
                return FockBuilder::CalculateMulti_(ir, spin, ibegin, iend, jbegin, jend,
                                                    outbuffer, bufsize);
        }
};

} // close namespace modulebase
} // close namespace pulsar

#endif
