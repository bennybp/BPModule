/*! \file
 *
 * \brief The base, general exception for BPModule (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_GENERALEXCEPTION_HPP_
#define _GUARD_GENERALEXCEPTION_HPP_

#include <stdexcept>

#include <vector>
#include <string>
#include <utility>


namespace bpmodule {
namespace exception {

/*! \brief A base exception for BPModule
 *
 * Besides the "what" string, all information is stored as
 * a vector of string pairs.
 *
 * It is derived from std::exception so that some information
 * (the what string) is available from that interface. Details
 * require catching the GeneralException though.
 */
class GeneralException : public std::exception
{
    public:
        typedef std::pair<std::string, std::string> ExceptionInfoPair;
        typedef std::vector<ExceptionInfoPair> ExceptionInfo;


        /*! \brief Constructor
         * 
         * \param [in] whatstr Some descriptive string
         * \param [in] exinfo Pairs of strings with other information.
         */
        template<typename... Targs>
        GeneralException(std::string whatstr, Targs... exinfo)
        {
        
        }

        /*! \brief Constructor
         * 
         * \param [in] whatstr Some descriptive string
         * \param [in] exinfo All other information as a vector of string pairs.
         */
        GeneralException(std::string whatstr, ExceptionInfo exinfo = {});

        GeneralException()                                         = delete;
        GeneralException(const GeneralException & rhs)             = default;
        GeneralException(GeneralException && rhs)                  = default;
        GeneralException & operator=(GeneralException && rhs)      = default;
        GeneralException & operator=(const GeneralException & rhs) = default;
        virtual ~GeneralException()                                = default;


        /*! \brief Get all the additional information
         */  
        const ExceptionInfo & GetInfo(void) const noexcept;


        /*! \brief Get some specific additional information
         * 
         * If the field doesn't exist, a string "(field not found)" is returned instead.
         */  
        const char * GetField(const std::string & field) const noexcept;


        /*! \brief Add information to this exception object
         */ 
        void AppendInfo(const ExceptionInfo & toappend);


        /*! \brief Add information to this exception object
         */ 
        void AppendInfo(const std::string & key, const std::string & value);


        /*! \brief Add information to this exception object
         */ 
        template<typename... Targs>
        void AppendInfo(const std::string & key, const std::string & value, Targs... exinfo)
        {
            AppendInfo(key, value);
            AppendInfo(exinfo...);
        }


        /*! \brief Print out the "what" string
         */ 
        const char * what(void) const noexcept;


        /*! \brief Print out all details in a nice format
         */ 
        std::string ExceptionString(void) const;

    private:
        //! The "what" string
        std::string whatstr_;

        //! All additional information
        ExceptionInfo exinfo_;
};


} // close namespace exception
} // close namespace bpmodule
#endif
