#include <string>
#include <cstring>
#include <cstdio>
#include <stdexcept>

#include "Printf_wrap.hpp"


namespace vprintfcpp {
namespace detail {

// Mapping of basic types to their printf specifiers
template<typename T> struct PFTypeMap { };

#define DECLARE_PFTYPE(t, cast, length, pft) template<> struct PFTypeMap<t> { \
         static constexpr const char * pflength = length; \
         static constexpr const char * pftype = pft; \
         typedef cast cast_type; \
       };


DECLARE_PFTYPE(bool,               int,                 "",    "d")
DECLARE_PFTYPE(char,               char,                "",    "c")

DECLARE_PFTYPE(signed char,        signed char,         "hh",  "d")
DECLARE_PFTYPE(signed short,       signed short,        "h",   "d")
DECLARE_PFTYPE(signed int,         signed int,          "",    "d")
DECLARE_PFTYPE(signed long,        signed long,         "l",   "d")
DECLARE_PFTYPE(signed long long,   signed long long,    "ll",  "d")
DECLARE_PFTYPE(unsigned char,      unsigned char,       "hh",  "uoxX")
DECLARE_PFTYPE(unsigned short,     unsigned short,      "h",   "uoxX")
DECLARE_PFTYPE(unsigned int,       unsigned int,        "",    "uoxX")
DECLARE_PFTYPE(unsigned long,      unsigned long,       "l",   "uoxX")
DECLARE_PFTYPE(unsigned long long, unsigned long long,  "ll",  "uoxX")

DECLARE_PFTYPE(float,            double, "",  "fFeEaAgG")
DECLARE_PFTYPE(double,           double, "",  "fFeEaAgG")
DECLARE_PFTYPE(long double, long double, "L", "fFeEaAgG")

DECLARE_PFTYPE(const char *, const char *, "", "s")
DECLARE_PFTYPE(char *,       char *,       "", "s")
DECLARE_PFTYPE(std::string,  std::string,  "", "s")

DECLARE_PFTYPE(const void *, const void *, "", "p")
DECLARE_PFTYPE(void *,       void *,       "", "p")






////////////////////////////////
// Main handler function
////////////////////////////////
template<typename T>
void HandlePrintf_Internal_(std::string & fmt, T subst)
{
    static const int bufsize = 64;

    // should be fine for most substitutions
    char buf[bufsize];
    char * buftouse = buf;

    // 1. Find the number of characters that would be written
    const int n = snprintf(nullptr, 0, fmt.c_str(), subst);

    // 2. Write to buffer or heap
    const int neededsize = n+1; // includes null termination

    if(neededsize > bufsize)  // need a bigger buffer
        buftouse = new char[neededsize];

    const int n2 = snprintf(buftouse, neededsize, fmt.c_str(), subst);

    // these two conditions signal success
    if(n2 >= 0 && n2 < neededsize)
        fmt = std::string(buftouse); // copies to the output

    // delete the buffer now if we created it
    if(neededsize > bufsize) // we used a new buffer
        delete [] buftouse;

    if(n2 < 0 || n2 >= neededsize)
        throw std::runtime_error(std::string("Error here: ") + std::to_string(n2) + " " + std::to_string(n2));
}




////////////////////////////////
// Helpers
////////////////////////////////
template<typename T>
void HandlePrintf_Internal_(std::string & fmt,
                            const char * length,
                            char spec, T subst)
{
    typedef typename PFTypeMap<T>::cast_type cast_type;
    const char * pftype  = PFTypeMap<T>::pftype;
    const char * pflength = PFTypeMap<T>::pflength;

    fmt.reserve(fmt.size() + 5);

    if(strlen(length) == 0 && spec == '?') // auto deduce
    {
        fmt += pflength;
        fmt += pftype[0];  // first type = default
    }
    else
    {
        // given a length and type spec, is it valid?
        if(strcmp(length, pflength))  // != 0
            throw std::runtime_error("Bad length specifier for type");

        // see if spec occurs in pftype
        if(strchr(pftype, spec) == nullptr)
            throw std::runtime_error("Bad type specifier for type");

        fmt += length;
        fmt += spec;
    }

    HandlePrintf_Internal_(fmt, static_cast<cast_type>(subst));
}

//////////////////////////
// Overloads
//////////////////////////
// const char * , since we don't always want it to be %s
void HandlePrintf_Internal_(std::string & fmt, const char * length,
                            char spec, const char * ptr)
{
    if(spec == 's' || spec == '?')
        HandlePrintf_Internal_<const char *>(fmt, length, spec, ptr);
    else
        HandlePrintf_Internal_<void const *>(fmt, length, spec, ptr);
}

// char * , since we don't always want it to be %s
void HandlePrintf_Internal_(std::string & fmt, const char * length,
                            char spec, char * ptr)
{
    HandlePrintf_Internal_(fmt, length, spec, static_cast<const char *>(ptr));
}


// std::string - for convenience
void HandlePrintf_Internal_(std::string & fmt, const char * length,
                            char spec, const std::string & subst)
{
    HandlePrintf_Internal_(fmt, length, spec, subst.c_str());
}



/////////////////////////////////////////
// Explicitly instantiate the templates
/////////////////////////////////////////
#define DECLARE_TEMPLATE_HANDLEPRINTF(type) \
       template void HandlePrintf_Internal_<type>(std::string &, const char *, char, type);

DECLARE_TEMPLATE_HANDLEPRINTF(bool)
DECLARE_TEMPLATE_HANDLEPRINTF(char)
DECLARE_TEMPLATE_HANDLEPRINTF(unsigned char)
DECLARE_TEMPLATE_HANDLEPRINTF(signed char)
DECLARE_TEMPLATE_HANDLEPRINTF(unsigned short)
DECLARE_TEMPLATE_HANDLEPRINTF(signed short)
DECLARE_TEMPLATE_HANDLEPRINTF(unsigned int)
DECLARE_TEMPLATE_HANDLEPRINTF(signed int)
DECLARE_TEMPLATE_HANDLEPRINTF(unsigned long)
DECLARE_TEMPLATE_HANDLEPRINTF(signed long)
DECLARE_TEMPLATE_HANDLEPRINTF(unsigned long long)
DECLARE_TEMPLATE_HANDLEPRINTF(signed long long)

DECLARE_TEMPLATE_HANDLEPRINTF(float)
DECLARE_TEMPLATE_HANDLEPRINTF(double)
DECLARE_TEMPLATE_HANDLEPRINTF(long double)

DECLARE_TEMPLATE_HANDLEPRINTF(void const *)

#undef DECLARE_TEMPLATE_HANDLEPRINTF

} // close namespace detail
} // close namespace vprintfcpp
