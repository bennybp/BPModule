/*! \file
 *
 * \brief Various string formatting functions (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#include "bpmodule/util/Format.hpp"

namespace bpmodule {
namespace util {

// Instantiate some commonly-used templates
// Zero arguments
template void FormatStream<>(std::ostream &, const std::string &);
template std::string FormatString<>(const std::string &);

// One argument
#define DECLARE_TEMPLATE_FORMAT_1(type1) \
        template void FormatStream<type1>(std::ostream &, const std::string &, const type1 &); \
        template std::string FormatString<type1>(const std::string &, const type1 &);

// Two arguments
#define DECLARE_TEMPLATE_FORMAT_2(type1, type2) \
        template void FormatStream<type1, type2>(std::ostream &, const std::string &, const type1 &, const type2 &); \
        template std::string FormatString<type1, type2>(const std::string &, const type1 &, const type2 &);

DECLARE_TEMPLATE_FORMAT_1(std::string)
DECLARE_TEMPLATE_FORMAT_1(int)
DECLARE_TEMPLATE_FORMAT_1(unsigned int)
DECLARE_TEMPLATE_FORMAT_1(unsigned long)
DECLARE_TEMPLATE_FORMAT_1(double)

DECLARE_TEMPLATE_FORMAT_2(std::string, std::string)
DECLARE_TEMPLATE_FORMAT_2(std::string, int)
DECLARE_TEMPLATE_FORMAT_2(std::string, unsigned int)
DECLARE_TEMPLATE_FORMAT_2(std::string, unsigned long)
DECLARE_TEMPLATE_FORMAT_2(std::string, double)

DECLARE_TEMPLATE_FORMAT_2(int, std::string)
DECLARE_TEMPLATE_FORMAT_2(int, int)
DECLARE_TEMPLATE_FORMAT_2(int, unsigned int)
DECLARE_TEMPLATE_FORMAT_2(int, unsigned long)
DECLARE_TEMPLATE_FORMAT_2(int, double)

DECLARE_TEMPLATE_FORMAT_2(unsigned int, std::string)
DECLARE_TEMPLATE_FORMAT_2(unsigned int, int)
DECLARE_TEMPLATE_FORMAT_2(unsigned int, unsigned int)
DECLARE_TEMPLATE_FORMAT_2(unsigned int, unsigned long)
DECLARE_TEMPLATE_FORMAT_2(unsigned int, double)

DECLARE_TEMPLATE_FORMAT_2(unsigned long, std::string)
DECLARE_TEMPLATE_FORMAT_2(unsigned long, int)
DECLARE_TEMPLATE_FORMAT_2(unsigned long, unsigned int)
DECLARE_TEMPLATE_FORMAT_2(unsigned long, unsigned long)
DECLARE_TEMPLATE_FORMAT_2(unsigned long, double)

DECLARE_TEMPLATE_FORMAT_2(double, std::string)
DECLARE_TEMPLATE_FORMAT_2(double, int)
DECLARE_TEMPLATE_FORMAT_2(double, unsigned int)
DECLARE_TEMPLATE_FORMAT_2(double, unsigned long)
DECLARE_TEMPLATE_FORMAT_2(double, double)


#undef DECLARE_TEMPLATE_FORMAT_1
#undef DECLARE_TEMPLATE_FORMAT_2


} // close namespace util
} // close namespace bpmodule
