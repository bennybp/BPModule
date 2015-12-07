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
        Test_Base(unsigned long id);

        Test_Base(PyObject * self, unsigned long id);


        //! \brief Just test some functionality
        virtual void RunTest(void);



        /*! \brief Call RunTest() of another module
         *
         * \param [in] other Key of the other module in the database
         */ 
        virtual void CallRunTest(const std::string & other);



        //! Throw an exception
        virtual void Throw(void);



        /*! \brief Call Throw() of another module
         *
         * \param [in] other Key of the other module in the database
         */ 
        virtual void CallThrow(const std::string & other);


    private:
        virtual boost::python::object MoveToPyObject_(std::function<void(modulebase::ModuleBase *)> deleter);

};

} // close namespace modulebase
} // close namespace bpmodule

#endif
