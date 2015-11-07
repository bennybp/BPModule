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
#include <sstream>


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
         */
        GeneralException(const std::string & whatstr)
            : whatstr_(whatstr)
        { }



        /*! \brief Constructor
         *
         * \param [in] whatstr Some descriptive string
         * \param [in] exinfo Pairs of strings with other information.
         */
        template<typename... Targs>
        GeneralException(std::string whatstr, const Targs& ... exinfo)
            : whatstr_(whatstr)
        {
            // check that there is an even number of exinfo
            static_assert( (sizeof...(exinfo) % 2) == 0,
                           "Information being added to exception has an odd number of values. Must be key1, value1, key2, value2, etc...");
            AppendInfo(exinfo...);
        }


        /*! \brief Constructor, using another exception as a base
         *
         * Can be used to append additional information
         *
         * \param [in] gex Exception to copy
         * \param [in] exinfo Additional information. Must be an even number of strings
         */
        template<typename... Targs>
        GeneralException(const GeneralException & gex, const Targs&... exinfo)
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
        void AppendInfo(const std::string & key, const std::string & value);


        /*! \brief Add information to this exception object
         *
         * \p value is converted via stringstreams
         */
        template<typename T>
        void AppendInfo(const std::string & key, const T & value)
        {
            std::stringstream ss;
            ss << value;
            AppendInfo(key, ss.str());
        }



        /*! \brief Add information to this exception object
         * 
         * Used to terminate the parameter pack expansion (in some instances)
         */
        void AppendInfo(void);



        /*! \brief Add information to this exception object
         *
         * There must be an even number of arguments.
         */
        template<typename... Targs>
        void AppendInfo(const std::string & key, const std::string & value,
                        const Targs&... exinfo)
        {
            static_assert( (sizeof...(exinfo) % 2) == 0,
                           "Information being added to exception has an odd number of values. Must be key1, value1, key2, value2, etc...");
            AppendInfo(key, value);
            AppendInfo(exinfo...);
        }


        /*! \brief Add information to this exception object
         *
         * There must be an even number of arguments.
         */
        template<typename T, typename... Targs>
        void AppendInfo(const std::string & key, const T & value,
                        const Targs&... exinfo)
        {
            AppendInfo(key, value);
            AppendInfo(exinfo...);
        }



        /*! \brief Add information to this exception object
         *
         * Overload for a vector of strings. Each element will get its own line.
         *
         * There must be an even number of arguments.
         */
        template<typename T, typename... Targs>
        void AppendInfo(const std::string & key,
                        const std::vector<T> & value,
                        const Targs&... exinfo)
        {
            static_assert( (sizeof...(exinfo) % 2) == 0,
                           "Information being added to exception has an odd number of values. Must be key1, value1, key2, value2, etc...");
            std::string str = "(empty)";

            // this is done manually
            // to prevent a dependence on other core modules
            std::stringstream ss;
            if(value.size())
                ss << value[0];
            for(size_t i = 1; i < value.size(); i++)
                ss << "\n" << value[i];

            AppendInfo(key, ss.str());
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
