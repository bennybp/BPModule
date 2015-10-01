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
        GeneralException(std::string whatstr)
            : whatstr_(whatstr)
        { }



        /*! \brief Constructor
         * 
         * \param [in] whatstr Some descriptive string
         * \param [in] exinfo Pairs of strings with other information.
         */
        template<typename... Targs>
        GeneralException(std::string whatstr, Targs... exinfo)
            : whatstr_(whatstr)
        {
            // check that there is an even number of exinfo
            static_assert( (sizeof...(exinfo) % 2) == 0, 
                           "Information being added to exception has an odd number of values. Must be key1, value1, key2, value2, etc...");
            AppendInfo(exinfo...);
        }


        /*! \brief Construct from another object, plus additional info
         */ 
        template<typename... Targs>
        GeneralException(const GeneralException & gex, Targs... exinfo)
            : GeneralException(gex)
        {
            // check that there is an even number of exinfo
            static_assert( (sizeof...(exinfo) % 2) == 0, 
                           "Information being added to exception has an odd number of values. Must be key1, value1, key2, value2, etc...");
            AppendInfo(exinfo...);
        }



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
         * 
         * This gets called if there is an odd number of exinfo arguments
         */ 
        void AppendInfo(const std::string & value)
        {
        }

        /*! \brief Add information to this exception object
         */ 
        template<typename... Targs>
        void AppendInfo(const std::string & key, const std::string & value, Targs... exinfo)
        {
            static_assert( (sizeof...(exinfo) % 2) == 0, 
                           "Information being added to exception has an odd number of values. Must be key1, value1, key2, value2, etc...");
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
