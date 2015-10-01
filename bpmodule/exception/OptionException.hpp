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
         * \param [in] what Brief description of the error
         * \param [in] key  Key of the option
         * \param [in] desc Additional information
         */
        template<typename... Targs>
        OptionException(std::string what,
                        std::string key,
                        Targs... Fargs)
                              
            : GeneralException(what, "optionkey", key, Fargs...)
        { }
                


        template<typename... Targs>
        OptionException(const GeneralException & gex, std::string key, Targs... Fargs)
            : GeneralException(gex, "optionkey", key, Fargs...)
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
