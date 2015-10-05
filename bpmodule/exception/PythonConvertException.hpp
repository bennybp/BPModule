/*! \file
 *
 * \brief An exception for python conversion errors
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_PYTHONCONVERTEXCEPTION_HPP_
#define _GUARD_PYTHONCONVERTEXCEPTION_HPP_

#include "bpmodule/exception/GeneralException.hpp"

namespace bpmodule {
namespace exception {


/*! \brief An exception thrown when there is a problem with converting to/from python
 */
class PythonConvertException : public GeneralException
{
    public:
        /*! \brief Constructor
         *
         * \param [in] whatstr Brief description of the error
         * \param [in] fromtype  The source type of the conversion
         * \param [in] totype  The destination type of the conversion
         * \param [in] exinfo Additional information. Must be an even number of strings
         */
        template<typename... Targs>
        PythonConvertException(std::string whatstr,
                               std::string fromtype,
                               std::string totype,
                               Targs... exinfo)

            : GeneralException(whatstr, "fromtype", fromtype, "totype", totype, exinfo...)
        { }


        /*! \brief Constructor, using another exception as a base
         *
         * Can be used to append additional information
         *
         * \param [in] gex Exception to copy
         * \param [in] fromtype  The source type of the conversion
         * \param [in] totype  The destination type of the conversion
         * \param [in] exinfo Additional information. Must be an even number of strings
         */
        template<typename... Targs>
        PythonConvertException(const GeneralException & gex,
                               std::string fromtype,
                               std::string totype,
                               Targs... exinfo)
            : GeneralException(gex, "fromtype", fromtype, "totype", totype, exinfo...)
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
