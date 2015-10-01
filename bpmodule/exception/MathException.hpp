/*! \file
 *
 * \brief An exception for math functions
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_MATHEXCEPTION_HPP_
#define _GUARD_MATHEXCEPTION_HPP_

#include "bpmodule/exception/GeneralException.hpp"

namespace bpmodule {
namespace exception {


/*! \brief An exception thrown when there is a problem with a math function
 *
 * This may be thrown on validation errors, etc.
 */
class MathException : public GeneralException
{
    public:
        /*! \brief Constructor
         *
         * \param [in] what Brief description of the error
         * \param [in] desc Additional information
         */
        template<typename... Targs>
        MathException(std::string what,
                        Targs... Fargs)
                              
            : GeneralException(what, Fargs...)
        { }
        
        
        template<typename... Targs>
        MathException(const GeneralException & gex, Targs... Fargs)
            : GeneralException(gex, Fargs...)
        { }

        
        MathException()                                      = delete;     
        MathException(const MathException & rhs)             = default;     
        MathException(MathException && rhs)                  = default;     
        MathException & operator=(const MathException & rhs) = default;     
        MathException & operator=(MathException && rhs)      = default;     
        virtual ~MathException() = default;
};



}// close namespace exception
}// close namespace bpmodule



#endif
