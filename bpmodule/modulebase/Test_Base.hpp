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
        Test_Base(unsigned long id, 
                  modulelocator::ModuleLocator & mlocator,
                  modulelocator::ModuleInfo & minfo);

        //! \brief Just test some functionality
        virtual void RunTest(void) = 0;



        /*! \brief Call RunTest() of another module
         *
         * \param [in] other Key of the other module in the database
         */ 
        virtual void CallRunTest(const std::string & other) = 0;



        //! Throw an exception
        virtual void Throw(void) = 0;



        /*! \brief Call Throw() of another module
         *
         * \param [in] other Key of the other module in the database
         */ 
        virtual void CallThrow(const std::string & other) = 0;



        /*! \brief Calculate some data and return it
         * 
         * \param [in] inputs Some calculation inputs
         * \return Some calculated data
         */
        virtual datastore::CalcData CalcTest(datastore::CalcData inputs) = 0;
};

} // close namespace modulebase
} // close namespace bpmodule

#endif
