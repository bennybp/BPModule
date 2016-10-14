/*! \file
 *
 * \brief Base class for a test module (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef PULSAR_GUARD_MODULEBASE__TEST_BASE_HPP_
#define PULSAR_GUARD_MODULEBASE__TEST_BASE_HPP_

#include "pulsar/modulebase/ModuleBase.hpp"


namespace pulsar{
namespace modulebase {

/*! \brief A test module
 *
 * Just to make sure the library works and provides a base class for C++
 * interface tests.
 */
class Test_Base : public ModuleBase
{
    public:
        typedef Test_Base BaseType;///< The type of the base


        Test_Base(ID_t id)
            : ModuleBase(id, "Test_Base")
        { }


        /*! \brief Just test some functionality
         */
        void run_test(void)
        {
            return ModuleBase::call_function(&Test_Base::run_test_);
        }


        /*! \brief Call run_test() of another module
         *
         * \param [in] other Key of the other module in the database
         */
        void call_run_test(const std::string & other)
        {
            return ModuleBase::call_function(&Test_Base::call_run_test_, other);
        }


        /*! \brief Nested call_run_test()
         *
         * Loads the module under other1 and then runs call_run_test(other2)
         * from that module.
         *
         * \param [in] other1 Key of the other module in the database
         * \param [in] other2 Key of the other module in the database
         */
        void call_run_test2(const std::string & other1, const std::string & other2)
        {
            return ModuleBase::call_function(&Test_Base::call_run_test2_, other1, other2);
        }


        /*! \brief Throw an exception
         */
        void test_throw(void)
        {
            return ModuleBase::call_function(&Test_Base::test_throw_);
        }



        /*! \brief Call Throw() of another module
         *
         * \param [in] other Key of the other module in the database
         */
        void call_throw(const std::string & other)
        {
            return ModuleBase::call_function(&Test_Base::call_throw_, other);
        }




        /////////////////////////////////////////
        // To be implemented by derived classes
        /////////////////////////////////////////
        /*! \copydoc run_test
         *
         * \note To be implemented by derived classes
         */
        virtual void run_test_(void) = 0;


        /*! \copydoc call_run_test
         *
         * \note To be implemented by derived classes
         */
        virtual void call_run_test_(const std::string & other) = 0;


        /*! \copydoc call_run_test2
         *
         * \note To be implemented by derived classes
         */
        virtual void call_run_test2_(const std::string & other1, const std::string & other2) = 0;


        /*! \copydoc test_throw
         *
         * \note To be implemented by derived classes
         */
        virtual void test_throw_(void) = 0;


        /*! \copydoc call_throw
         *
         * \note To be implemented by derived classes
         */
        virtual void call_throw_(const std::string & other) = 0;


};




class Test_Base_Py : public Test_Base
{
    public:
        using Test_Base::Test_Base;

        MODULEBASE_FORWARD_PROTECTED_TO_PY


        virtual void run_test_(void)
        {
            return call_py_override<void>("run_test_");
        }

        virtual void call_run_test_(const std::string & other)
        {
            return call_py_override<void>("call_run_test_", other);
        }

        virtual void call_run_test2_(const std::string & other1, const std::string & other2)
        {
            return call_py_override<void>("call_run_test2_", other1, other2);
        }

        virtual void test_throw_(void)
        {
            return call_py_override<void>("test_throw_");
        }

        virtual void call_throw_(const std::string & other)
        {
            return call_py_override<void>("call_throw_", other);
        }

};


} // close namespace modulebase
} // close namespace pulsar

#endif
