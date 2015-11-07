/*! \file
 *
 * \brief An exception for options handling
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_PYTHONCALLEXCEPTION_HPP_
#define _GUARD_PYTHONCALLEXCEPTION_HPP_

#include "bpmodule/exception/GeneralException.hpp"

namespace bpmodule {
namespace exception {


/*! \brief An exception thrown when there is a problem calling a python function
 */
class PythonCallException : public GeneralException
{
    public:
        /*! \brief Constructor
         *
         * \param [in] whatstr Brief description of the error
         * \param [in] pyex Exception string from python
         * \param [in] exinfo Additional information. Must be an even number of strings
         */
        template<typename... Targs>
        PythonCallException(std::string whatstr,
                            std::string pyex,
                            const Targs&... exinfo)

            : GeneralException(whatstr, "pyex", pyex, exinfo...)
        { }


        /*! \brief Constructor, using another exception as a base
         *
         * Can be used to append additional information
         *
         * \param [in] gex Exception to copy
         * \param [in] pyex Exception string from python
         * \param [in] exinfo Additional information. Must be an even number of strings
         */
        template<typename... Targs>
        PythonCallException(const GeneralException & gex,
                            std::string pyex,
                            const Targs&... exinfo)
            : GeneralException(gex, "pyex", pyex, exinfo...)
        { }

        PythonCallException()                                            = delete;
        PythonCallException(const PythonCallException & rhs)             = default;
        PythonCallException(PythonCallException && rhs)                  = default;
        PythonCallException & operator=(const PythonCallException & rhs) = default;
        PythonCallException & operator=(PythonCallException && rhs)      = default;
        virtual ~PythonCallException() = default;
};



}// close namespace exception
}// close namespace bpmodule



#endif
