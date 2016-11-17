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
class PulsarException : public std::exception
{
  public:
    PulsarException() = delete;
    PulsarException(const PulsarException &) = default;
    PulsarException(PulsarException &&) = default;
    PulsarException & operator=(PulsarException &&) = default;
    PulsarException & operator=(const PulsarException &) = default;
    virtual ~PulsarException() = default;

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
    PulsarException(const std::string & whatstr, const Targs &... exinfo)
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
    PulsarException(const std::exception & ex, const Targs &... exinfo)
      : PulsarException(ex.what(), exinfo...)
    {
    }

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
    void build_str_(std::string &) noexcept
    {
    }

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

} // close namespace pulsar
