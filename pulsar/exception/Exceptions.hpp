/*! \file
 * \brief Exceptions thrown by Pulsar (header)
 */

#pragma once

#include <stdexcept>
#include <vector>
#include <string>
#include <sstream>


namespace pulsar{
namespace exception {

/*! \brief A base exception for Pulsar
 *
 * An exception in Pulsar is basically a string with some fancy formatting.
 * There is one line (an overall description) followed by pairs of
 * information. This is all available via the what() member, which will
 * print this information in a nicely-formatted manner.
 *
 * An exception in Pulsar is basically a string with some fancy formatting.
 * There is one line (an overall description) followed by pairs of
 * information. This is all available via the what() member, which will
 * return this information as a nicely-formatted manner.
 *
 * After construction, pairs of information can be added to the exception via
 * the `append_info` functions.
 */
class GeneralException : public std::exception
{
    public:
        GeneralException()                                     = delete;
        GeneralException(const GeneralException &)             = default;
        GeneralException(GeneralException &&)                  = default;
        GeneralException & operator=(GeneralException &&)      = default;
        GeneralException & operator=(const GeneralException &) = default;
        virtual ~GeneralException()                            = default;


        /*! \brief Constructor
         *
         * The extra information passed to this function via \p exinfo must be
         * in the form "key1, value1, key2, value2, ...". The keys must be
         * strings and the values must be a string or convertible to a string
         * via a c++ stream object.
         *
         * \param [in] whatstr Some string describing the exception
         * \param [in] exinfo Additional information to add to the exception
         */
        template<typename... Targs>
        GeneralException(const std::string & whatstr, const Targs& ... exinfo)
            : whatstr_(whatstr)
        {
            append_info_(exinfo...);
        }


        /*! \brief Constructor, using an std::exception as a base
         *
         * The `what()` value of the `std::exception` is used as the main
         * description of the exception.
         *
         * The extra information passed to this function via \p exinfo must be
         * in the form "key1, value1, key2, value2, ...". The keys must be
         * strings and the values must be a string or convertible to a string
         * via a c++ stream object.
         *
         * \param [in] ex Exception to copy
         * \param [in] exinfo Additional information too add to the exception
         */
        template<typename... Targs>
        GeneralException(const std::exception & ex, const Targs&... exinfo)
            : GeneralException(ex.what(), exinfo...)
        { }


        /* \brief Add information to this exception object
         *
         * The extra information passed to this function via \p exinfo must be
         * in the form "key1, value1, key2, value2, ...". The keys must be
         * strings and the values must be a string or convertible to a string
         * via a c++ stream object.
         */
        template<typename... Targs>
        void append_info(const Targs&... exinfo)
        {
            static_assert( (sizeof...(exinfo) % 2) == 0,
                           "Information being added to exception has an odd"
                           " number of values. Arguments must be in the form"
                           " key1, value1, key2, value2, ...");

            append_info_(exinfo...);
        }


        /*! \brief Return all the information as a string
         *
         * The string will be formatted for output, with the description on
         * its own line and each key,value pair on its own line.
         */
        const char * what(void) const noexcept;


    private:
        /*! A string representing all the information contained
            in this exception, formatted for output.
         */
        std::string whatstr_;


        /*! \brief Add information to this exception object
         *
         * \note Used to terminate the parameter pack expansion
         *       in some instances
         */
        void append_info_(void) noexcept { };


        /*! \brief Add a single pair of information to exception object
         *
         * \param [in] key A description of what \p value means
         * \param [in] value An informative string expected to help the user
         *                   or developer.
         */
        void append_info_(const std::string & key, const std::string & value);


        /*! \brief Add a single pair of information to exception object
         *
         * The \p value must be a string or convertible to a string
         * via a c++ stream object.
         *
         * \param [in] key A description of what \p value means
         * \param [in] value Information expected to help the user
         *                   or developer.
         */
        template<typename T>
        void append_info_(const std::string & key, const T & value)
        {
            std::stringstream ss;
            ss << value;
            append_info_(key, ss.str());
        }



        /*! \brief Add information to this exception object
         *
         * The \p value must be a string or convertible to a string
         * via a c++ stream object.
         *
         * \param [in] key A description of what \p value means
         * \param [in] value Information expected to help the user
         *                   or developer.
         */
        template<typename T, typename... Targs>
        void append_info_(const std::string & key, const T & value,
                          const Targs&... exinfo)
        {
            append_info_(key, value);
            append_info_(exinfo...);
        }


        /*! \brief Add information to this exception object
         *
         * Used to add a vector of information. Each element of \p value
         * will get its own line, with the key only appearing on the first.
         *
         * The elements of \p value must be a string or convertible to a string
         * via a c++ stream object.
         *
         * If \p value does not contain any elements, the string "(empty)" is
         * used instead.
         */
        template<typename T, typename... Targs>
        void append_info_(const std::string & key,
                          const std::vector<T> & value,
                          const Targs&... exinfo)
        {
            std::string str = "(empty)";

            std::stringstream ss;
            if(value.size())
                ss << value[0];
            for(size_t i = 1; i < value.size(); i++)
                ss << "\n" << value[i];

            append_info_(key, ss.str());
            append_info_(exinfo...);
        }
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
class ModuleManagerException : public GeneralException
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
class SystemException : public GeneralException
{
    public:
        using GeneralException::GeneralException;
};


/*! \brief An exception thrown when there is a problem with
 *         serialization/unserialization
 */
class SerializationException : public GeneralException
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
} // close namespace pulsar
