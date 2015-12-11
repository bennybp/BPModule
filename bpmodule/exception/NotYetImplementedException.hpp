/*! \file
 *
 * \brief An exception for module creation errors
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_NOTYETIMPLEMENTEDEXCEPTION_HPP_
#define _GUARD_NOTYETIMPLEMENTEDEXCEPTION_HPP_

#include "bpmodule/exception/GeneralException.hpp"

namespace bpmodule {
namespace exception {


/*! \brief An exception thrown when something is not implemented
 */
class NotYetImplementedException : public GeneralException
{
    public:
        /*! \brief Constructor
         *
         * \param [in] whatstr What isn't implemented
         * \param [in] exinfo Additional information. Must be an even number of strings
         */
        template<typename... Targs>
        NotYetImplementedException(const std::string & whatstr,
                                   const Targs&... exinfo)
            : GeneralException(std::string("Not yet implemented: ") + whatstr, exinfo...)
        { }



        NotYetImplementedException()                                              = delete;
        NotYetImplementedException(const NotYetImplementedException & rhs)             = default;
        NotYetImplementedException(NotYetImplementedException && rhs)                  = default;
        NotYetImplementedException & operator=(const NotYetImplementedException & rhs) = default;
        NotYetImplementedException & operator=(NotYetImplementedException && rhs)      = default;
        virtual ~NotYetImplementedException() = default;
};



}// close namespace exception
}// close namespace bpmodule



#endif
