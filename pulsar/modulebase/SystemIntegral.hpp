/*! \file
 *
 * \brief Integrals that deal only with System (header)
 * \author Ben Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_MODULEBASE__SYSTEMINTEGRAL_HPP_
#define PULSAR_GUARD_MODULEBASE__SYSTEMINTEGRAL_HPP_

#include "pulsar/modulebase/ModuleBase.hpp"


namespace pulsar{
namespace modulebase {

/*! \brief One-electron integral implementation
 *
 */
class SystemIntegral : public ModuleBase
{
    public:
        typedef SystemIntegral BaseType;

        SystemIntegral(ID_t id)
            : ModuleBase(id, "SystemIntegral"), initialized_(false)
        { }


        /*! \brief initialize the integral computation
         *
         * \param [in] deriv Derivative to calculate
         * \param [in] sys System for which we are calculating the integrals
         */
        void initialize(unsigned int deriv, const system::System & sys)
        {
            ModuleBase::call_function(&SystemIntegral::uninitialized_or_throw_);
            ModuleBase::call_function(&SystemIntegral::initialize_, deriv, sys);
            initialized_ = true;
        }


        /*! \brief calculate an integral
         *
         * \param [in] deriv Derivative to calculate
         * \param [in] outbuffer Where to place the completed integrals
         * \param [in] bufsize Size of \p outbuffer (as the number of doubles)
         * \return Number of integrals calculated
         */
        uint64_t calculate(double * outbuffer, size_t bufsize)
        {
            ModuleBase::call_function(&SystemIntegral::initialized_or_throw_);
            return ModuleBase::call_function(&SystemIntegral::calculate_,
                                                outbuffer, bufsize);
        }


        /*! \brief calculate an integral (for use from python)
         *
         * \param [in] deriv Derivative to calculate
         * \param [in] shell1 Shell index on the first center
         * \param [in] shell2 Shell index on the second center
         * \param [in] outbuffer Where to place the completed integrals
         * \return Number of integrals calculated
         */
        uint64_t calculate_py(pybind11::buffer outbuffer)
        {
            auto ptrinfo = python_buffer_to_ptr<double>(outbuffer, 1);

            return ModuleBase::call_function(&SystemIntegral::calculate_,
                                                  ptrinfo.first, ptrinfo.second[0]);
        }


        /////////////////////////////////////////
        // To be implemented by derived classes
        /////////////////////////////////////////

        //! \copydoc initialize
        virtual void initialize_(unsigned int deriv, const system::System & sys) = 0;

        //! \copydoc calculate
        virtual uint64_t calculate_(double * outbuffer, size_t bufsize) = 0;



    private:
        bool initialized_; //!< Has initialize() been called


        void initialized_or_throw_(void) const
        {
            if(!initialized_)
                throw exception::GeneralException("Module is not yet initialized");
        }

        void uninitialized_or_throw_(void) const
        {
            if(initialized_)
                throw exception::GeneralException("Module has already been initialized");
        }
};


class SystemIntegral_Py : public SystemIntegral
{
    public:
        using SystemIntegral::SystemIntegral;

        MODULEBASE_FORWARD_PROTECTED_TO_PY

        virtual void initialize_(unsigned int deriv, const system::System & sys)
        {
            return call_py_override<void>(this, "initialize_", deriv, sys);
        }

        virtual uint64_t calculate_(double * outbuffer, size_t bufsize)
        {
            //! \todo untested, won't work
            pybind11::buffer_info buf(outbuffer,
                                      sizeof(double),
                                      pybind11::format_descriptor<double>::value,
                                      1, { bufsize },
                                      { sizeof(double) });

            return call_py_override<uint64_t>(this, "calculate_", buf, bufsize);
        }
};

} // close namespace modulebase
} // close namespace pulsar

#endif
