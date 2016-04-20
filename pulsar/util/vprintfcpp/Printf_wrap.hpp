#ifndef VPRINTFCPP__PRINTF_WRAP_HPP_
#define VPRINTFCPP__PRINTF_WRAP_HPP_

#include <string>


namespace vprintfcpp {

namespace detail {

/////////////////////////////////////
// Internal HandlePrintf_Internal_ functions
/////////////////////////////////////
template<typename T>
void HandlePrintf_Internal_(std::string & fmt, const char * length,
                            char spec, T subst);

// For pointer types - convert to void *
template<typename T>
void HandlePrintf_Internal_(std::string & fmt, const char * length,
                            char spec, T * ptr)
{
    return HandlePrintf_Internal_<void const *>(fmt, length, spec, ptr);
}


//////////////////
// Overloads
//////////////////
void HandlePrintf_Internal_(std::string & fmt, const char * length,
                            char spec, const std::string & subst);

void HandlePrintf_Internal_(std::string & fmt, const char * length, 
                            char spec, char * subst);

void HandlePrintf_Internal_(std::string & fmt, const char * length, 
                            char spec, const char * subst);


/////////////////////////////////////////////////////////
// Valid arguments are explicitly instantiated
// and marked as valid with the ValidPrintfArg trait
// structure.
/////////////////////////////////////////////////////////

template<typename T> struct ValidPrintfArg : public std::false_type { };

#define DECLARE_VALID_PRINTF(type) \
    extern template void HandlePrintf_Internal_<type>(std::string &, const char *, char, type); \
    template<> struct ValidPrintfArg<type> : public std::true_type { };

DECLARE_VALID_PRINTF(bool)
DECLARE_VALID_PRINTF(char)
DECLARE_VALID_PRINTF(unsigned char)
DECLARE_VALID_PRINTF(signed char)
DECLARE_VALID_PRINTF(unsigned short)
DECLARE_VALID_PRINTF(signed short)
DECLARE_VALID_PRINTF(unsigned int)
DECLARE_VALID_PRINTF(signed int)
DECLARE_VALID_PRINTF(unsigned long)
DECLARE_VALID_PRINTF(signed long)
DECLARE_VALID_PRINTF(unsigned long long)
DECLARE_VALID_PRINTF(signed long long)

DECLARE_VALID_PRINTF(float)
DECLARE_VALID_PRINTF(double)
DECLARE_VALID_PRINTF(long double)

DECLARE_VALID_PRINTF(void const *)

#undef DECLARE_VALID_PRINTF


// And mark the overloads as valid
template<> struct ValidPrintfArg<std::string> : public std::true_type { };
template<> struct ValidPrintfArg<char *> : public std::true_type { };
template<> struct ValidPrintfArg<const char *> : public std::true_type { };

// pointers
template<typename T> struct ValidPrintfArg<T *> : public std::true_type { };


/////////////////////////////////////////
// To be used from the outside
/////////////////////////////////////////
template<typename T>
void HandlePrintf_(std::string & fmt, const char * length, char spec, T subst)
{
    return HandlePrintf_Internal_(fmt, length, spec, static_cast<typename std::decay<T>::type>(subst));
}


} // close namespace detail
} // close namespace vprintfcpp




#endif
