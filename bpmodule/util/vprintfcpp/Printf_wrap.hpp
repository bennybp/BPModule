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


////////////////////////////
// Explicitly instantiated
// Only handles basic types
////////////////////////////
#define DECLARE_EXTERN_TEMPLATE_HANDLEPRINTF(type) \
    extern template void HandlePrintf_Internal_<type>(std::string &, const char *, char, type);

DECLARE_EXTERN_TEMPLATE_HANDLEPRINTF(bool)
DECLARE_EXTERN_TEMPLATE_HANDLEPRINTF(char)
DECLARE_EXTERN_TEMPLATE_HANDLEPRINTF(unsigned char)
DECLARE_EXTERN_TEMPLATE_HANDLEPRINTF(signed char)
DECLARE_EXTERN_TEMPLATE_HANDLEPRINTF(unsigned short)
DECLARE_EXTERN_TEMPLATE_HANDLEPRINTF(signed short)
DECLARE_EXTERN_TEMPLATE_HANDLEPRINTF(unsigned int)
DECLARE_EXTERN_TEMPLATE_HANDLEPRINTF(signed int)
DECLARE_EXTERN_TEMPLATE_HANDLEPRINTF(unsigned long)
DECLARE_EXTERN_TEMPLATE_HANDLEPRINTF(signed long)
DECLARE_EXTERN_TEMPLATE_HANDLEPRINTF(unsigned long long)
DECLARE_EXTERN_TEMPLATE_HANDLEPRINTF(signed long long)

DECLARE_EXTERN_TEMPLATE_HANDLEPRINTF(float)
DECLARE_EXTERN_TEMPLATE_HANDLEPRINTF(double)
DECLARE_EXTERN_TEMPLATE_HANDLEPRINTF(long double)

DECLARE_EXTERN_TEMPLATE_HANDLEPRINTF(void const *)

#undef DECLARE_EXTERN_TEMPLATE_HANDLEPRINTF

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
