/*! \file
 *
 * \brief The base, general exception for BPModule (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_EXCEPTION__EXCEPTIONS_HPP_
#define BPMODULE_GUARD_EXCEPTION__EXCEPTIONS_HPP_

#include <stdexcept>

#include <vector>
#include <string>
#include <sstream>


namespace bpmodule {
namespace exception {

/*! \brief A base exception for BPModule
 *
 * Basically, this is a string with some fancy formatting.
 * There is one line (basically an overall description)
 * followed by pairs of information. This is all available
 * via the what() member.
 *
 * Pairs of information can be appended to the string via
 * AppendInfo().
 *
 * When constructed from another std::exception, the what() string
 * of that is copied first.
 */
class GeneralException : public std::exception
{
    public:
        /*! \brief Constructor
         *
         * \param [in] whatstr Some descriptive string
         * \param [in] exinfo Pairs of strings with other information.
         */
        template<typename... Targs>
        GeneralException(const std::string & whatstr, const Targs& ... exinfo)
            : whatstr_(whatstr)
        {
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
        GeneralException(const std::exception & ex, const Targs&... exinfo)
            : GeneralException(ex.what(), exinfo...)
        {
        }



        GeneralException()                                         = delete;
        GeneralException(const GeneralException & rhs)             = default;
        GeneralException(GeneralException && rhs)                  = default;
        GeneralException & operator=(GeneralException && rhs)      = default;
        GeneralException & operator=(const GeneralException & rhs) = default;
        virtual ~GeneralException()                                = default;


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
            static_assert( (sizeof...(exinfo) % 2) == 0,
                           "Information being added to exception has an odd number of values. Must be key1, value1, key2, value2, etc...");
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


        /*! \brief Return all the information as a string
         */
        const char * what(void) const noexcept;


    private:

        //! The "what" string
        std::string whatstr_;
};



/*! \brief An exception thrown if something is called that
 *         hasn't been implemented yet
 */
class NotYetImplementedException : public GeneralException
{
    public:
        using GeneralException::GeneralException;
};

/*! \brief An exception thrown if a value is out of range
 */
class ValueOutOfRange : public GeneralException
{
    public:
        using GeneralException::GeneralException;
};

/*! \brief An exception thrown when there is a problem with a
 *         basis set or basis functions
 */
class BasisSetException : public GeneralException
{
    public:
        using GeneralException::GeneralException;
};



/*! \brief An exception thrown when there is a problem with
 *         data storage classes
 */
class DataStoreException : public GeneralException
{
    public:
        using GeneralException::GeneralException;
};



/*! \brief An exception thrown when there is a problem with
 *         some math operations
 */
class MathException : public GeneralException
{
    public:
        using GeneralException::GeneralException;
};



/*! \brief An exception thrown when there is a problem with
 *         creating/instantiating a module
 */
class ModuleCreateException : public GeneralException
{
    public:
        using GeneralException::GeneralException;
};


/*! \brief An exception thrown when there is a problem with
 *         the module locator
 */
class ModuleLocatorException : public GeneralException
{
    public:
        using GeneralException::GeneralException;
};


/*! \brief An exception thrown when there is a problem with
 *         loading/inserting a module
 */
class ModuleLoadException : public GeneralException
{
    public:
        using GeneralException::GeneralException;
};


/*! \brief An exception thrown when there is a problem with
 *         molecule/system operations
 */
class MoleculeException : public GeneralException
{
    public:
        using GeneralException::GeneralException;
};


/*! \brief An exception thrown when there is a problem with
 *         options
 */
class OptionException : public GeneralException
{
    public:
        using GeneralException::GeneralException;
};


/*! \brief An exception thrown when there is a problem with
 *         calling a python function from C++
 */
class PythonCallException : public GeneralException
{
    public:
        using GeneralException::GeneralException;
};


/*! \brief An exception thrown when there is a problem with
 *         converting to and from python data types
 */
class PythonConvertException : public GeneralException
{
    public:
        using GeneralException::GeneralException;
};



} // close namespace exception
} // close namespace bpmodule

#endif
