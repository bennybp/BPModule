/*! \file
 *
 * \brief Base class for a test module (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_MODULEBASE__TEST_BASE_HPP_
#define BPMODULE_GUARD_MODULEBASE__TEST_BASE_HPP_

#include "bpmodule/modulebase/ModuleBase.hpp"


namespace bpmodule {
namespace modulebase {

/*! \brief A test module
 *
 * Just to make sure the library works
 */
class Test_Base : public ModuleBase
{
    public:
        typedef Test_Base BaseType;


        Test_Base(ID_t id)
            : ModuleBase(id, "Test_Base")
        { }


        /*! \brief Just test some functionality
         */
        void RunTest(void)
        {
            return ModuleBase::CallFunction(&Test_Base::RunTest_);
        }


        /*! \brief Call RunTest() of another module
         *
         * \param [in] other Key of the other module in the database
         */
        void CallRunTest(const std::string & other)
        {
            return ModuleBase::CallFunction(&Test_Base::CallRunTest_, other);
        }


        /*! \brief Nested CallRunTest()
         *
         * Loads the module under other1 and then runs CallRunTest(other2)
         * from that module.
         *
         * \param [in] other1 Key of the other module in the database
         * \param [in] other2 Key of the other module in the database
         */
        void CallRunTest2(const std::string & other1, const std::string & other2)
        {
            return ModuleBase::CallFunction(&Test_Base::CallRunTest2_, other1, other2);
        }


        /*! \brief Throw an exception
         */
        void TestThrow(void)
        {
            return ModuleBase::CallFunction(&Test_Base::TestThrow_);
        }



        /*! \brief Call Throw() of another module
         *
         * \param [in] other Key of the other module in the database
         */
        void CallThrow(const std::string & other)
        {
            return ModuleBase::CallFunction(&Test_Base::CallThrow_, other);
        }




        /////////////////////////////////////////
        // To be implemented by derived classes
        /////////////////////////////////////////
        /*! \copydoc RunTest
         *
         * \note To be implemented by derived classes
         */
        virtual void RunTest_(void) = 0;


        /*! \copydoc CallRunTest
         *
         * \note To be implemented by derived classes
         */
        virtual void CallRunTest_(const std::string & other) = 0;


        /*! \copydoc CallRunTest2
         *
         * \note To be implemented by derived classes
         */
        virtual void CallRunTest2_(const std::string & other1, const std::string & other2) = 0;


        /*! \copydoc TestThrow
         *
         * \note To be implemented by derived classes
         */
        virtual void TestThrow_(void) = 0;


        /*! \copydoc CallThrow
         *
         * \note To be implemented by derived classes
         */
        virtual void CallThrow_(const std::string & other) = 0;


};




class Test_Base_Py : public Test_Base
{
    public:
        using Test_Base::Test_Base;

        MODULEBASE_FORWARD_PROTECTED_TO_PY


        virtual void RunTest_(void)
        {
            return CallPyOverride<void>("RunTest_");
        }



        virtual void CallRunTest_(const std::string & other)
        {
            return CallPyOverride<void>("CallRunTest_", other);
        }

        virtual void CallRunTest2_(const std::string & other1, const std::string & other2)
        {
            return CallPyOverride<void>("CallRunTest2_", other1, other2);
        }



        virtual void TestThrow_(void)
        {
            return CallPyOverride<void>("TestThrow_");
        }



        virtual void CallThrow_(const std::string & other)
        {
            return CallPyOverride<void>("CallThrow_", other);
        }

};


} // close namespace modulebase
} // close namespace bpmodule

#endif
