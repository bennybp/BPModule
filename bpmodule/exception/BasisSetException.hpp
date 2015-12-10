/*! \file
 *
 * \brief An exception for molecules and atoms
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_BASISSETEXCEPTION_HPP_
#define _GUARD_BASISSETEXCEPTION_HPP_

#include "bpmodule/exception/GeneralException.hpp"

namespace bpmodule {
namespace exception {


/*! \brief An exception thrown when there is a problem with a basis set or basis functions
 */
class BasisSetException : public GeneralException
{
    public:
        /*! \brief Constructor
         *
         * \param [in] whatstr Brief description of the error
         * \param [in] exinfo Additional information. Must be an even number of strings
         */
        template<typename... Targs>
        BasisSetException(const std::string & whatstr,
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
        BasisSetException(const GeneralException & gex, const Targs&... exinfo)
            : GeneralException(gex, exinfo...)
        { }


        BasisSetException()                                      = delete;
        BasisSetException(const BasisSetException & rhs)             = default;
        BasisSetException(BasisSetException && rhs)                  = default;
        BasisSetException & operator=(const BasisSetException & rhs) = default;
        BasisSetException & operator=(BasisSetException && rhs)      = default;
        virtual ~BasisSetException() = default;
};



}// close namespace exception
}// close namespace bpmodule



#endif
