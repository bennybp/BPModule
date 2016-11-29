/*! \file
 *
 * \brief Base class for a test module (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_MODULEBASE__TEST_BASE_HPP_
#define PULSAR_GUARD_MODULEBASE__TEST_BASE_HPP_

#include "pulsar/modulebase/ModuleBase.hpp"


namespace pulsar{

/*! \brief A test module
 *
 * This module serves to provide a means of testing module-module interactions.
 * It is also used as a base for C++ tests of the various Pulsar classes.
 * Through the derived class: Test_CXX_Base
 * 
 * \todo Is it OK to make all but run_test_ null functions in this class?
 */
class TestModule : public ModuleBase
{
    public:
        typedef TestModule BaseType;///< The type of the base

        ///Just forwards arguments to base
        TestModule(ID_t id)
            : ModuleBase(id, "TestModule")
        { }


        /*! \brief Just test some functionality
         * 
         *  For C++ tests of the core functionality this is the function that
         *  will be called which means you need to override run_test_
         */
        void run_test(void)
        {
            return ModuleBase::call_function(&TestModule::run_test_);
        }


        /////////////////////////////////////////
        // To be implemented by derived classes
        /////////////////////////////////////////
        /*! \copydoc run_test
         *
         * \note To be implemented by derived classes
         */
        virtual void run_test_(void) = 0;

};

class TestModule_Py : public TestModule
{
    public:
        using TestModule::TestModule;

        MODULEBASE_FORWARD_PROTECTED_TO_PY


        virtual void run_test_(void)
        {
            return call_py_override<void>(this, "run_test_");
        }
};

} // close namespace pulsar

#endif
