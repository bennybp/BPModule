/*! \file
 *
 * \brief An exception for options handling
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_OPTIONEXCEPTION_HPP_
#define _GUARD_OPTIONEXCEPTION_HPP_

#include "bpmodule/exception/GeneralException.hpp"

namespace bpmodule {
namespace exception {


/*! \brief An exception thrown when there is a problem with options
 *
 * This may be thrown on validation errors, etc.
 */
class OptionException : public GeneralException
{
    public:
        /*! \brief Constructor
         *
         * \param [in] whatstr Brief description of the error
         * \param [in] key  Key of the option
         * \param [in] exinfo Additional information. Must be an even number of strings
         */
        template<typename... Targs>
        OptionException(const std::string & whatstr,
                        std::string key,
                        const Targs&... exinfo)

            : GeneralException(whatstr, "optionkey", key, exinfo...)
        { }



        /*! \brief Constructor, using another exception as a base
         *
         * Can be used to append additional information
         *
         * \param [in] gex Exception to copy
         * \param [in] key  Key of the option
         * \param [in] exinfo Additional information. Must be an even number of strings
         */
        template<typename... Targs>
        OptionException(const GeneralException & gex, std::string key, const Targs&... exinfo)
            : GeneralException(gex, "optionkey", key, exinfo...)
        { }


        OptionException()                                        = delete;
        OptionException(const OptionException & rhs)             = default;
        OptionException(OptionException && rhs)                  = default;
        OptionException & operator=(const OptionException & rhs) = default;
        OptionException & operator=(OptionException && rhs)      = default;
        virtual ~OptionException() = default;


        //! Return the key of the option with problems
        const char * Key(void) const noexcept     { return GeneralException::GetField("optionkey");  }
};



}// close namespace exception
}// close namespace bpmodule



#endif
