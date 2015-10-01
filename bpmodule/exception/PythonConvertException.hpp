/*! \file
 *
 * \brief An exception for options handling
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_PYTHONCONVERTEXCEPTION_HPP_
#define _GUARD_PYTHONCONVERTEXCEPTION_HPP_

#include "bpmodule/exception/GeneralException.hpp"

namespace bpmodule {
namespace exception {


/*! \brief An exception thrown when there is a problem with converting to/from python
 *
 * \note This should never be propagated to the user. Generally, conversions should be
 *       tested first, and then run. If that last conversion fails, this is thrown, so
 *       it should be treated as sort of a developer error.
 */
class PythonConvertException : public GeneralException
{
    public:
        /*! \brief Constructor
         *
         * \param [in] what Brief description of the error
         * \param [in] key  Key of the option
         * \param [in] desc Additional information
         */
        template<typename... Targs>
        PythonConvertException(std::string what,
                               std::string fromtype, std::string totype,
                               Targs... Fargs)
                              
            : GeneralException(what, "fromtype", fromtype, "totype", totype, Fargs...)
        { }
        
        
        template<typename... Targs>
        PythonConvertException(const GeneralException & gex,
                               std::string fromtype, std::string totype,
                               Targs... Fargs)
            : GeneralException(gex, "fromtype", fromtype, "totype", totype, Fargs...)
        { }
        
        PythonConvertException()                                               = delete;     
        PythonConvertException(const PythonConvertException & rhs)             = default;     
        PythonConvertException(PythonConvertException && rhs)                  = default;     
        PythonConvertException & operator=(const PythonConvertException & rhs) = default;     
        PythonConvertException & operator=(PythonConvertException && rhs)      = default;     
        virtual ~PythonConvertException() = default;
};



}// close namespace exception
}// close namespace bpmodule



#endif
