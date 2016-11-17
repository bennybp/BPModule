/*! \file
 * \brief Exceptions thrown by Pulsar (header)
 */

#pragma once

#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace pulsar {

/*! \brief A base exception for Pulsar
 *
 * An exception in Pulsar is basically a string with some fancy formatting.
 * There is one line (an overall description) followed by pairs of
 * information. This is all available via the what() member, which will
 * print this information in a nicely-formatted manner.
 *
 * After construction, pairs of information can be added to the exception via
 * the `append_info` function.
 *
 * For more thorough documentation on how exceptions should be used,
 * see \ref except
 *
 * \threadunsafe
 */
class GeneralException : public std::exception
{
  public:
    GeneralException() = delete;
    GeneralException(const GeneralException &) = default;
    GeneralException(GeneralException &&) = default;
    GeneralException & operator=(GeneralException &&) = default;
    GeneralException & operator=(const GeneralException &) = default;
    virtual ~GeneralException() = default;


    /*! \brief Constructor
     *
     * The extra information passed to this function via \p exinfo must be
     * in the form "key1, value1, key2, value2, ...". The keys must be
     * strings and the values must be a string or convertible to a string
     * via a C++ stream object.
     *
     * \exstrong
     *
     * \param [in] whatstr Some string describing the problem
     * \param [in] exinfo Additional information to add to the exception
     */
    template<typename... Targs>
    GeneralException(const std::string & whatstr, const Targs &... exinfo)
      : whatstr_(whatstr)
    {
        build_str_(whatstr_, exinfo...);
    }


    /*! \brief Constructor, using an std::exception as a base
     *
     * The `what()` value of the `std::exception` is used as the main
     * description of the exception.
     *
     * The extra information passed to this function via \p exinfo must be
     * in the form "key1, value1, key2, value2, ...". The keys must be
     * strings and the values must be a string or convertible to a string
     * via a C++ stream object.
     *
     * \exstrong
     *
     * \param [in] ex Standard exception from which to get the description
     * \param [in] exinfo Additional information too add to the exception
     */
    template<typename... Targs>
    GeneralException(const std::exception & ex, const Targs &... exinfo)
      : GeneralException(ex.what(), exinfo...)
    { }


    /*! \brief Add information to this exception object
     *
     * The extra information passed to this function via \p exinfo must be
     * in the form "key1, value1, key2, value2, ...". The keys must be
     * strings and the values must be a string or convertible to a string
     * via a C++ stream object.
     *
     * \exstrong
     *
     * \param [in] exinfo Additional information to add to the exception
     */
    template<typename... Targs>
    void append_info(const Targs &... exinfo)
    {
        static_assert((sizeof...(exinfo) % 2) == 0,
                      "Information being added to exception has an odd"
                      " number of values. Arguments must be in the form"
                      " key1, value1, key2, value2, ...");

        build_str_(whatstr_, exinfo...);
    }


    /*! \brief Return all the information as a string
     *
     * The string will be formatted for output, with the description on
     * its own line, follow by every key,value pair on its own line.
     *
     * \exnothrow
     */
    const char * what(void) const noexcept
    {
        return whatstr_.c_str();
    }


  private:
    /*! A string representing all the information contained
        in this exception, formatted for output.
     */
    std::string whatstr_;


    /*! \brief Add information to this exception object
     *
     * \exnothrow
     *
     * \note Used to terminate the parameter pack expansion
     *       in some instances
     */
    void build_str_(std::string &) noexcept {}


    /*! \brief Add a single pair of information to a string
     *
     * \exstrong
     *
     * \param [inout] outstr The string to append to
     * \param [in] key A description of what \p value means
     * \param [in] value An informative string expected to help the user
     *                   or developer.
     */
    static void build_str_(std::string & outstr,
                           const std::string & key,
                           const std::string & value);


    /*! \brief Add a single pair of information to a string
     *
     * The \p value argument must be a string or convertible to a string
     * via a C++ stream object.
     *
     * \exstrong
     *
     * \param [inout] outstr The string to append to
     * \param [in] key A description of what \p value means
     * \param [in] value Information expected to help the user
     *                   or developer.
     */
    template<typename T>
    static void build_str_(std::string & outstr,
                           const std::string & key,
                           const T & value)
    {
        std::stringstream ss;
        ss << value;
        build_str_(outstr, key, ss.str());
    }


    /*! \brief Add information to this exception object
     *
     * The \p value must be a string or convertible to a string
     * via a C++ stream object.
     *
     * \exstrong
     *
     * \param [inout] outstr The string to append to
     * \param [in] key A description of what \p value means
     * \param [in] value Information expected to help the user
     *                   or developer.
     */
    template<typename T, typename... Targs>
    static void build_str_(std::string & outstr,
                           const std::string & key,
                           const T & value,
                           const Targs &... exinfo)
    {
        // done this way for strong exception safety
        std::string out_tmp;
        build_str_(out_tmp, key, value);
        build_str_(out_tmp, exinfo...);

        outstr.append(out_tmp);
    }


    /*! \brief Add information to this exception object
     *
     * Used to add a vector of information. Each element of \p values
     * will get its own line, with the key only appearing on the first.
     *
     * The elements of \p values must be a string or convertible to a string
     * via a C++ stream object.
     *
     * If \p values does not contain any elements, the string "(empty)" is
     * used instead.
     *
     * The rest of the \p exinfo arguments are passed to other append_info()
     * functions.
     *
     * \exstrong
     *
     * \param [in] key A description of what \p values means
     * \param [in] values A vector of data that is expected to help the user
     *                 or developer.
     * \param [in] exinfo Additional information to add to the exception
     */
    template<typename T, typename... Targs>
    static void build_str_(std::string & outstr,
                           const std::string & key,
                           const std::vector<T> & values,
                           const Targs &... exinfo)
    {
        if(values.size() == 0)
            build_str_(outstr, key, "(empty)", exinfo...);
        else
        {
            std::stringstream ss;
            ss << values[0];

            for(size_t i = 1; i < values.size(); i++)
                ss << "\n" << values[i];

            build_str_(outstr, key, ss.str(), exinfo...);
        }
    }
};


#define PULSAR_EXCEPTION(cls) \
class cls : public GeneralException \
{\
  public: \
    using GeneralException::GeneralException;\
};


/*! \brief An exception thrown if something is called that
 *         hasn't been implemented yet.
 *
 *  See \ref except
 */
PULSAR_EXCEPTION(NotYetImplementedException)

/*! \brief An exception thrown if a value is out of some range.
 *
 *  See \ref except
 */
PULSAR_EXCEPTION(ValueOutOfRange)

/*! \brief An exception thrown when there is a problem with a
 *         basis set or basis functions.
 *
 *  See \ref except
 */
PULSAR_EXCEPTION(BasisSetException)

/*! \brief An exception thrown when there is a problem with
 *         data storage classes.
 *
 *  See \ref except
 */
PULSAR_EXCEPTION(DataStore)

/*! \brief An exception thrown when there is a problem with
 *         some math operations.
 *
 *  See \ref except
 */
PULSAR_EXCEPTION(MathException)

/*! \brief An exception thrown when there is a problem with
 *         creating/instantiating a module.
 *
 *  See \ref except
 */
PULSAR_EXCEPTION(ModuleCreateException)

/*! \brief An exception thrown when there is a problem with
 *         the module locator.
 *
 *  See \ref except
 */
PULSAR_EXCEPTION(ModuleManagerException)

/*! \brief An exception thrown when there is a problem with
 *         loading/inserting a module.
 *
 *  See \ref except
 */
PULSAR_EXCEPTION(ModuleLoadException)

/*! \brief An exception thrown when there is a problem with
 *         molecule/system operations.
 *
 *  See \ref except
 */
PULSAR_EXCEPTION(SystemException)

/*! \brief An exception thrown when there is a problem with
 *         serialization/unserialization.
 *
 *  See \ref except
 */
PULSAR_EXCEPTION(SerializationException)

/*! \brief An exception thrown when there is a problem with
 *         options.
 *
 *  See \ref except
 */
PULSAR_EXCEPTION(OptionException)

/*! \brief An exception thrown when there is a problem with
 *         calling a python function from C++.
 *
 *  See \ref except
 */
PULSAR_EXCEPTION(PythonCallException)

/*! \brief An exception thrown when there is a problem with
 *         converting to and from python data types.
 *
 *  See \ref except
 */
PULSAR_EXCEPTION(PythonConvertException)

/*! \brief An exception thrown if there is a null pointer.
 *
 *  See \ref except
 */
PULSAR_EXCEPTION(NullPointerException)

#undef PULSAR_EXCEPTION


} // close namespace pulsar
