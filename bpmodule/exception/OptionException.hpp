/*! \file
 *
 * \brief An exception for map/dictionary errors
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_OPTIONEXCEPTION_HPP_
#define _GUARD_OPTIONEXCEPTION_HPP_

#include "bpmodule/exception/GeneralException.hpp"

namespace bpmodule {
namespace exception {


/*! \brief An exception thrown when there is a problem with options
 *
 */
class OptionException : public GeneralException
{
    public:
        /*! \brief Constructor
         *
         * \param [in] what Brief description of the error
         * \param [in] type Type that had a problem
         * \param [in] desc Additional information
         */
        OptionException(std::string what,
                        std::string type,
                        std::string desc = "(no details)")
            : GeneralException(what,
                               {
                                  {"type", type},
                                  {"desc", desc}
                               })
                { }
        
        
        OptionException()                                        = delete;     
        OptionException(const OptionException & rhs)             = default;     
        OptionException(OptionException && rhs)                  = default;     
        OptionException & operator=(const OptionException & rhs) = default;     
        OptionException & operator=(OptionException && rhs)      = default;     
        virtual ~OptionException() = default;


        //! Return the type that had the problem
        const char * Type(void) const noexcept     { return GeneralException::GetField("type"); }

        //! Return any additional details
        const char * Desc(void) const noexcept     { return GeneralException::GetField("desc"); }
};



}// close namespace bpmodule
}// close namespace exception



#endif
