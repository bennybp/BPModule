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
 * This may be thrown on validation errors, overflows, etc.
 */
class MathException : public GeneralException
{
    public:
        /*! \brief Constructor
         *
         * \param [in] whatstr Brief description of the error
         * \param [in] exinfo Additional information. Must be an even number of strings
         */
        template<typename... Targs>
        MathException(const std::string & whatstr,
                        const Targs&... exinfo)

            : GeneralException(whatstr, exinfo...)
        { }


        /*! \brief Constructor, using another exception as a base
         *
         * Can be used to append additional information
         *
         * \param [in] gex Exception to copy
         * \param [in] exinfo Additional information. Must be an even number of strings
         */
        template<typename... Targs>
        MathException(const GeneralException & gex, const Targs&... exinfo)
            : GeneralException(gex, exinfo...)
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
