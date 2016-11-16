/*! \file
 *
 * \brief Various string formatting functions (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#pragma once

#include <bpprint/Format.hpp>
#include "pulsar/exception/Exceptions.hpp"


namespace pulsar{

/*! \brief Output a formatted string to a stream
 *
 * \throw pulsar::GeneralException for malformed inputs, etc
 *
 * \tparam Targs The types of the arguments to print
 *
 * \param [in] os The stream to output to
 * \param [in] fmt The format string to use
 * \param [in] Fargs The arguments to the format string
 */
template<typename... Targs>
void format_stream(std::ostream & os, const std::string & fmt,
                  const Targs&... Fargs)
{
    try {
      bpprint::format_stream(os, fmt, Fargs...);
    }
    catch(std::exception & ex)
    {
        throw GeneralException("Error in formatting a string or stream",
                                          "error", ex.what(),
                                          "fmt", fmt, "nargs", sizeof...(Fargs));
    }
}



/*! \brief Create a formatted string
 *
 * \throw pulsar::GeneralException for malformed inputs, etc
 *
 * \tparam Targs The types of the arguments to print
 *
 * \param [in] fmt The format string to use
 * \param [in] Fargs The arguments to the format string
 * \return a Formatted string
 */
template<typename... Targs>
std::string format_string(const std::string & fmt, const Targs&... Fargs)
{
    try {
      return bpprint::format_string(fmt, Fargs...);
    }
    catch(std::exception & ex)
    {
        throw GeneralException("Error in formatting a string or stream",
                                          "error", ex.what(),
                                          "fmt", fmt, "nargs", sizeof...(Fargs));
    }
}





// Instantiate some commonly-used templates
// Zero arguments
extern template void format_stream<>(std::ostream &, const std::string &);
extern template std::string format_string<>(const std::string &);

// One argument
#define DECLARE_EXTERN_TEMPLATE_FORMAT_1(type1) \
        extern template void format_stream<type1>(std::ostream &, const std::string &, const type1 &); \
        extern template std::string format_string<type1>(const std::string &, const type1 &);

// Two arguments
#define DECLARE_EXTERN_TEMPLATE_FORMAT_2(type1, type2) \
        extern template void format_stream<type1, type2>(std::ostream &, const std::string &, const type1 &, const type2 &); \
        extern template std::string format_string<type1, type2>(const std::string &, const type1 &, const type2 &);

DECLARE_EXTERN_TEMPLATE_FORMAT_1(std::string)
DECLARE_EXTERN_TEMPLATE_FORMAT_1(int)
DECLARE_EXTERN_TEMPLATE_FORMAT_1(unsigned int)
DECLARE_EXTERN_TEMPLATE_FORMAT_1(unsigned long)
DECLARE_EXTERN_TEMPLATE_FORMAT_1(double)

DECLARE_EXTERN_TEMPLATE_FORMAT_2(std::string, std::string)
DECLARE_EXTERN_TEMPLATE_FORMAT_2(std::string, int)
DECLARE_EXTERN_TEMPLATE_FORMAT_2(std::string, unsigned long)
DECLARE_EXTERN_TEMPLATE_FORMAT_2(std::string, double)
DECLARE_EXTERN_TEMPLATE_FORMAT_2(std::string, unsigned int)

DECLARE_EXTERN_TEMPLATE_FORMAT_2(int, std::string)
DECLARE_EXTERN_TEMPLATE_FORMAT_2(int, int)
DECLARE_EXTERN_TEMPLATE_FORMAT_2(int, unsigned int)
DECLARE_EXTERN_TEMPLATE_FORMAT_2(int, unsigned long)
DECLARE_EXTERN_TEMPLATE_FORMAT_2(int, double)

DECLARE_EXTERN_TEMPLATE_FORMAT_2(unsigned int, std::string)
DECLARE_EXTERN_TEMPLATE_FORMAT_2(unsigned int, int)
DECLARE_EXTERN_TEMPLATE_FORMAT_2(unsigned int, unsigned int)
DECLARE_EXTERN_TEMPLATE_FORMAT_2(unsigned int, unsigned long)
DECLARE_EXTERN_TEMPLATE_FORMAT_2(unsigned int, double)

DECLARE_EXTERN_TEMPLATE_FORMAT_2(unsigned long, std::string)
DECLARE_EXTERN_TEMPLATE_FORMAT_2(unsigned long, int)
DECLARE_EXTERN_TEMPLATE_FORMAT_2(unsigned long, unsigned int)
DECLARE_EXTERN_TEMPLATE_FORMAT_2(unsigned long, unsigned long)
DECLARE_EXTERN_TEMPLATE_FORMAT_2(unsigned long, double)

DECLARE_EXTERN_TEMPLATE_FORMAT_2(double, std::string)
DECLARE_EXTERN_TEMPLATE_FORMAT_2(double, int)
DECLARE_EXTERN_TEMPLATE_FORMAT_2(double, unsigned int)
DECLARE_EXTERN_TEMPLATE_FORMAT_2(double, unsigned long)
DECLARE_EXTERN_TEMPLATE_FORMAT_2(double, double)

#undef DECLARE_EXTERN_TEMPLATE_FORMAT_1
#undef DECLARE_EXTERN_TEMPLATE_FORMAT_2

} // close namespace pulsar

