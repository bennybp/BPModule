#ifndef _GUARD_PYTHONCONVERTEXCEPTION_HPP_
#define _GUARD_PYTHONCONVERTEXCEPTION_HPP_

#include "bpmodule/exception/GeneralException.hpp"

namespace bpmodule {
namespace exception {


/*! \brief An exception thrown when a module object can not be created
 *
 * Particularly when creating modules or looking up information keys
 */
class PythonConvertException : public GeneralException
{
    public:
        /*! \brief Constructor
         *
         * \param [in] what     Brief description of the error
         * \param [in] location Where the exception occurred
         * \param [in] key      Key that couldn't be converted
         * \param [in] from     Source type
         * \param [in] to       Destination type
         * \param [in] desc     Additional information
         */
        PythonConvertException(std::string what,
                               std::string location,
                               std::string key,
                               std::string from,
                               std::string to,
                               std::string desc = "(no details)")
            : GeneralException(what,
                               {
                                  {"location", location},
                                  {"key", key},
                                  {"from", from},
                                  {"to", to},
                                  {"desc", desc}
                               })
                { }

        /*! \brief Constructor with limited information
         *
         * \param [in] what     Brief description of the error
         * \param [in] from     Source type
         * \param [in] to       Destination type
         * \param [in] desc     Additional information
         */
        PythonConvertException(std::string what,
                               std::string from,
                               std::string to,
                               std::string desc = "(no details)")
            : GeneralException(what,
                               {
                                  {"from", from},
                                  {"to", to},
                                  {"desc", desc}
                               })
                { }
                
        
        PythonConvertException()                                               = delete;     
        PythonConvertException(const PythonConvertException & rhs)             = default;     
        PythonConvertException(PythonConvertException && rhs)                  = default;     
        PythonConvertException & operator=(const PythonConvertException & rhs) = default;     
        PythonConvertException & operator=(PythonConvertException && rhs)      = default;     
        virtual ~PythonConvertException() = default;


        //! Return where the exception occurred
        const char * Location(void) const noexcept    { return GeneralException::GetField("path"); }

        //! Return the key that couldn't be converted
        const char * Key(void) const noexcept     { return GeneralException::GetField("key");  }

        //! Return the type of what was being converted
        const char * From(void) const noexcept    { return GeneralException::GetField("from"); }

        //! Return the destination type
        const char * To(void) const noexcept    { return GeneralException::GetField("to"); }

        //! Return any additional details
        const char * Desc(void) const noexcept    { return GeneralException::GetField("desc"); }
};



}// close namespace bpmodule
}// close namespace exception



#endif
