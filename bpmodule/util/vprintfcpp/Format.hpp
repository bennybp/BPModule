#ifndef VPRINTFCPP__FORMAT_HPP_
#define VPRINTFCPP__FORMAT_HPP_

#include <sstream>
#include <stdexcept>

#include "bpmodule/util/vprintfcpp/Printf_wrap.hpp"

namespace vprintfcpp {


namespace detail {


/*! \brief Information about a single format specification
 */
struct FormatInfo
{
    std::string prefix; //!< The string before the format specification
    std::string suffix; //!< The string after the format specification
    std::string format; //!< The format specification itself, except for the length and type specifier characters
    char length[3];     //!< The length specifier
    char spec;          //!< The type specifier character
};


/*! \brief Get the next format specification
 *
 * If the function returns true, the members of \p fi are filled in.
 *
 * If the function returns false, the prefix member contains the string but
 * with some processing (such as replacing %% with %).
 *
 * \throw std::runtime_error if the format string is badly formatted
 *
 * \param [out] fi Information about the specification
 * \param [in] str The string to search
 * \return True if a format specification was found, otherwise false
 */
bool GetNextFormat_(FormatInfo & fi, const std::string & str);




/*! \brief Format a string into an ostream
 *
 * Used to terminate the variadic template
 *
 * \throw std::runtime_error if the string contains a format
 *        specification (meaning it is expecting an argument)
 *
 * \param [in] os The ostream used to build the string
 * \param [in] fi Format info to use as a workspace
 * \param [in] str String (possibly with format string specification)
 */ 
void Format_(std::ostream & os, FormatInfo & fi, const std::string & str);



/*! \brief Format a string into an ostream
 *
 * This will only format the first specification found in
 * \p str, using \p arg as the substitution
 *
 * \throw std::runtime_error if the correct number of arguments is not given or
 *        if the format string is badly formed
 *
 * \param [in] os The ostream used to build the string
 * \param [in] fi The format information struct to use
 * \param [in] str String (possibly with format string specification)
 * \param [in] arg Substitution for the first format specification found
 * \param [in] args Additional arguments for later format specifications
 */ 
template<typename T, typename... Targs>
void Format_(std::ostream & os, FormatInfo & fi,
             const std::string & str, const T & arg, const Targs&... args)
{
    if(GetNextFormat_(fi, str)) 
    {
        // after this, fi.format has been overwritten
        HandlePrintf_(fi.format, fi.length, fi.spec, arg);

        os << fi.prefix << fi.format;
        Format_(os, fi, fi.suffix, args...);
    }
    else
        throw std::runtime_error("Too many arguments to format string");
}

} // close namespace detail


/* \brief Apply formatting to a string, outputting it to a stream
 *
 * \throw std::runtime_error if the correct number of arguments is not given or
 *        if the format string is badly formed
 */
template<typename... Targs>
void FormatStream(std::ostream & os, const std::string & str, const Targs&... args)
{
    detail::FormatInfo fi;

    //reserve space in the strings
    fi.prefix.reserve(32);
    fi.suffix.reserve(32);
    fi.format.reserve(16);

    detail::Format_(os, fi, str, args...);
}



/* \brief Apply formatting to a string
 *
 * \throw std::runtime_error if the correct number of arguments is not given or
 *        if the format string is badly formed
 */
template<typename... Targs>
std::string FormatString(const std::string & str, const Targs&... args)
{
    std::stringstream ss;
    FormatStream(ss, str, args...);
    return ss.str();
}



} // close namespace vprintfcpp

#endif
