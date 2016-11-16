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
class Test_Base : public ModuleBase
{
    public:
        typedef Test_Base BaseType;///< The type of the base

        ///Just forwards arguments to base
        Test_Base(ID_t id)
            : ModuleBase(id, "Test_Base")
        { }


        /*! \brief Just test some functionality
         * 
         *  For C++ tests of the core functionality this is the function that
         *  will be called which means you need to override run_test_
         */
        void run_test(void)
        {
            return ModuleBase::call_function(&Test_Base::run_test_);
        }


        /*! \brief Call run_test() of another module
         *
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



        /*! \brief Add something to this module's cache
         * 
         * \param [in] policy The policy of the cache element to add
         */
        void add_to_cache(const std::string & key, unsigned int policy)
        {
            return ModuleBase::call_function(&Test_Base::add_to_cache_, key, policy);
        }

        /*! \brief Get something from this module's cache
         * 
         * \param [in] policy The policy of the cache element to add
         */
        void get_from_cache(const std::string & key, bool use_distcache)
        {
            return ModuleBase::call_function(&Test_Base::get_from_cache_, key, use_distcache);
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


        /*! \copydoc add_to_cache
         *
         * \note To be implemented by derived classes
         */
        virtual void add_to_cache_(const std::string & key, unsigned int policy) = 0;


        /*! \copydoc get_from_cache
         *
         * \note To be implemented by derived classes
         */
        virtual void get_from_cache_(const std::string & key, bool use_distcache) = 0;


};

///Implements all but run_test_
class Test_CXX_Base : public Test_Base {
public:
    using Test_Base::Test_Base;
protected:
        void call_run_test_(const std::string &){}
        void call_run_test2_(const std::string &, const std::string &){}
        void test_throw_(void){}
        void call_throw_(const std::string &){}
        void add_to_cache_(const std::string &, unsigned int){}
        void get_from_cache_(const std::string &, bool){}
};




class Test_Base_Py : public Test_Base
{
    public:
        using Test_Base::Test_Base;

        MODULEBASE_FORWARD_PROTECTED_TO_PY


        virtual void run_test_(void)
        {
            return call_py_override<void>(this, "run_test_");
        }

        virtual void call_run_test_(const std::string & other)
        {
            return call_py_override<void>(this, "call_run_test_", other);
        }

        virtual void call_run_test2_(const std::string & other1, const std::string & other2)
        {
            return call_py_override<void>(this, "call_run_test2_", other1, other2);
        }

        virtual void test_throw_(void)
        {
            return call_py_override<void>(this, "test_throw_");
        }

        virtual void call_throw_(const std::string & other)
        {
            return call_py_override<void>(this, "call_throw_", other);
        }

        virtual void add_to_cache_(const std::string & key, unsigned int policy)
        {
            return call_py_override<void>(this, "add_to_cache_", key, policy);
        }

        virtual void get_from_cache_(const std::string & key, bool use_distcache)
        {
            return call_py_override<void>(this, "get_from_cache_", key, use_distcache);
        }


};

} // close namespace pulsar

#endif
