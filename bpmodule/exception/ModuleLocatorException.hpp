/*! \file
 *
 * \brief An exception for module store errors
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_MODULELOCATOREXCEPTION_HPP_
#define _GUARD_MODULELOCATOREXCEPTION_HPP_

#include "bpmodule/exception/GeneralException.hpp"

namespace bpmodule {
namespace exception {


/*! \brief An exception thrown when there is a problem in the ModuleLocator
 */
class ModuleLocatorException : public GeneralException
{
    public:
        /*! \brief Constructor
         *
         * \param [in] whatstr Brief description of the error
         * \param [in] exinfo Additional information. Must be an even number of strings
         */
        template<typename... Targs>
        ModuleLocatorException(std::string whatstr,
                             Targs... exinfo)
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
        ModuleLocatorException(const GeneralException & gex,
                              Targs... exinfo)
            : GeneralException(gex, exinfo...)
        { }


        ModuleLocatorException()                                             = delete;
        ModuleLocatorException(const ModuleLocatorException & rhs)             = default;
        ModuleLocatorException(ModuleLocatorException && rhs)                  = default;
        ModuleLocatorException & operator=(const ModuleLocatorException & rhs) = default;
        ModuleLocatorException & operator=(ModuleLocatorException && rhs)      = default;
        virtual ~ModuleLocatorException() = default;

};



}// close namespace exception
}// close namespace bpmodule



#endif
