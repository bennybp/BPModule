/*! \file
 *
 * \brief Base class for a test module (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_TEST_BASE_HPP_
#define _GUARD_TEST_BASE_HPP_

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
        Test_Base(unsigned long id)
            : ModuleBase(id)
        { }

        Test_Base(PyObject * self, unsigned long id)
            : ModuleBase(self, id)
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
        virtual void RunTest_(void)
        {
            return ModuleBase::CallPyMethod<void>("RunTest_");
        }


        /*! \copydoc CallRunTest
         * 
         * \note To be implemented by derived classes
         */ 
        virtual void CallRunTest_(const std::string & other)
        {
            return ModuleBase::CallPyMethod<void>("CallRunTest_", other);
        }


        /*! \copydoc TestThrow
         * 
         * \note To be implemented by derived classes
         */ 
        virtual void TestThrow_(void)
        {
            return ModuleBase::CallPyMethod<void>("TestThrow_");
        }


        /*! \copydoc CallThrow
         * 
         * \note To be implemented by derived classes
         */ 
        virtual void CallThrow_(const std::string & other)
        {
            return ModuleBase::CallPyMethod<void>("CallThrow_", other);
        }



    private:
        virtual boost::python::object MoveToPyObject_(std::function<void(modulebase::ModuleBase *)> deleter)
        {
            return ModuleBase::MoveToPyObjectHelper_<Test_Base>(deleter, this);
        }

};

} // close namespace modulebase
} // close namespace bpmodule

#endif
