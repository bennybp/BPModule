/*! \file
 *
 * \brief An exception for module store errors
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_MODULESTOREEXCEPTION_HPP_
#define _GUARD_MODULESTOREEXCEPTION_HPP_

#include "bpmodule/exception/GeneralException.hpp"

namespace bpmodule {
namespace exception {


/*! \brief An exception thrown when there is a problem in the ModuleStore
 */
class ModuleStoreException : public GeneralException
{
    public:
        /*! \brief Constructor
         *
         * \param [in] whatstr Brief description of the error
         * \param [in] exinfo Additional information. Must be an even number of strings
         */
        template<typename... Targs>
        ModuleStoreException(std::string whatstr,
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
        ModuleStoreException(const GeneralException & gex,
                              Targs... exinfo)
            : GeneralException(gex, exinfo...)
        { }


        ModuleStoreException()                                             = delete;
        ModuleStoreException(const ModuleStoreException & rhs)             = default;
        ModuleStoreException(ModuleStoreException && rhs)                  = default;
        ModuleStoreException & operator=(const ModuleStoreException & rhs) = default;
        ModuleStoreException & operator=(ModuleStoreException && rhs)      = default;
        virtual ~ModuleStoreException() = default;

};



}// close namespace exception
}// close namespace bpmodule



#endif
