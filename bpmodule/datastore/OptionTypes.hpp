/*! \file
 *
 * \brief Safe conversions option types
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_DATASTORE__OPTIONTYPES_HPP_
#define BPMODULE_GUARD_DATASTORE__OPTIONTYPES_HPP_


#include <cstdint>

#include "bpmodule/math/Cast.hpp"
#include "bpmodule/math/Cast_stl.hpp"

namespace bpmodule {
namespace datastore {


////////////////////////////////////////////////
// Storage types for integral and floating point
////////////////////////////////////////////////

//! Integer options are stored as this type
typedef intmax_t OptionInt;

//! Floating point options are stored as this type
typedef long double OptionFloat;




////////////////////////////////////////////////
// Available option types
////////////////////////////////////////////////
enum class OptionType
{
    Int, Float, Bool, String,
    SetInt, SetFloat, SetBool, SetString,
    ListInt, ListFloat, ListBool, ListString
};



inline const char * OptionTypeToString(OptionType ot)
{
    switch(ot)
    {
        case OptionType::Int:
            return "Int";
        case OptionType::Float:
            return "Float";
        case OptionType::Bool:
            return "Bool";
        case OptionType::String:
            return "String";
        case OptionType::ListInt:
            return "ListInt";
        case OptionType::ListFloat:
            return "ListFloat";
        case OptionType::ListBool:
            return "ListBool";
        case OptionType::ListString:
            return "ListString";
        case OptionType::SetInt:
            return "SetInt";
        case OptionType::SetFloat:
            return "SetFloat";
        case OptionType::SetBool:
            return "SetBool";
        case OptionType::SetString:
            return "SetString";
        default:
            throw std::logic_error("Undefined OptionType");
    }
}



///////////////////////////////////////
// Information about an OptionType
///////////////////////////////////////
template<OptionType T> struct OptionTypeInfo { };

#define MAKEOPTIONTYPE(OPTTYPE, STOREDTYPE) \
        template<> struct OptionTypeInfo<OptionType::OPTTYPE> { \
        typedef STOREDTYPE stored_type; };

MAKEOPTIONTYPE(Int,         OptionInt)
MAKEOPTIONTYPE(Float,       OptionFloat)
MAKEOPTIONTYPE(Bool,        bool)
MAKEOPTIONTYPE(String,      std::string)

MAKEOPTIONTYPE(ListInt,     std::vector<OptionInt>)
MAKEOPTIONTYPE(ListFloat,   std::vector<OptionFloat>)
MAKEOPTIONTYPE(ListBool,    std::vector<bool>)
MAKEOPTIONTYPE(ListString,  std::vector<std::string>)

MAKEOPTIONTYPE(SetInt,      std::set<OptionInt>)
MAKEOPTIONTYPE(SetFloat,    std::set<OptionFloat>)
MAKEOPTIONTYPE(SetBool,     std::set<bool>)
MAKEOPTIONTYPE(SetString,   std::set<std::string>)



////////////////////////////////////////
// Mapping of types to the option type
// Kind of long. Could be replaced with some
// tag dispatch, but this is probably easier
// and not that many more lines
////////////////////////////////////////
template<typename T> struct OptionTypeMap { static constexpr bool valid = false; };

#define MAPTOOPTIONTYPE(TYPE, OPTTYPE) \
        template<> struct OptionTypeMap<TYPE> { \
        static constexpr OptionType opt_type = OptionType::OPTTYPE; \
        static constexpr bool valid = true; }; \
        template<> struct OptionTypeMap<std::vector<TYPE>> { \
        static constexpr OptionType opt_type = OptionType::List##OPTTYPE; \
        static constexpr bool valid = true; }; \
        template<> struct OptionTypeMap<std::set<TYPE>> { \
        static constexpr OptionType opt_type = OptionType::Set##OPTTYPE; \
        static constexpr bool valid = true; }; \


MAPTOOPTIONTYPE(bool,                Bool)
MAPTOOPTIONTYPE(unsigned char,       Int)
MAPTOOPTIONTYPE(signed   char,       Int)
MAPTOOPTIONTYPE(unsigned short,      Int)
MAPTOOPTIONTYPE(signed   short,      Int)
MAPTOOPTIONTYPE(unsigned int,        Int)
MAPTOOPTIONTYPE(signed   int,        Int)
MAPTOOPTIONTYPE(unsigned long,       Int)
MAPTOOPTIONTYPE(signed   long,       Int)
MAPTOOPTIONTYPE(unsigned long long,  Int)
MAPTOOPTIONTYPE(signed   long long,  Int)
MAPTOOPTIONTYPE(float,               Float)
MAPTOOPTIONTYPE(double,              Float)
MAPTOOPTIONTYPE(long double,         Float)
MAPTOOPTIONTYPE(std::string,         String)




/////////////////////////////
// Conversion
/////////////////////////////

/*! \brief Converts an option value to/from its stored type
 *
 * Default struct is empty, therefore invalid for use
 */
template<typename Target, typename Source>
struct OptionCast
{
    // normally we just use numeric_cast
    // will work for vector types, etc
    static Target Cast(const Source & s)   {  return math::numeric_cast<Target, Source>(s);  }
};



/*! \brief Converts an option value to/from its stored type
 *
 * Override for string types
 */
template<>
struct OptionCast<std::string, std::string>
{
    static std::string Cast(const std::string & s)   {  return s; }
};

template<>
struct OptionCast<std::vector<std::string>, std::vector<std::string>>
{
    static std::vector<std::string> Cast(const std::vector<std::string> & s)   {  return s; }
};

template<>
struct OptionCast<std::set<std::string>, std::set<std::string>>
{
    static std::set<std::string> Cast(const std::set<std::string> & s)   {  return s; }
};




} // close namespace datastore
} // close namespace bpmodule

#endif
