/*! \file
 *
 * \brief Safe conversions option types
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef BPMODULE_GUARD_DATASTORE__OPTIONTYPES_HPP_
#define BPMODULE_GUARD_DATASTORE__OPTIONTYPES_HPP_


#include <cstdint>
#include <set>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

#include "bpmodule/math/Cast.hpp"
#include "bpmodule/math/Cast_stl.hpp"

namespace bpmodule {
namespace datastore {


////////////////////////////////////////////////
// Storage types for integral and floating point
////////////////////////////////////////////////

//! Integer options are stored as this type
typedef intmax_t OptionInt;


////////////////////////////////////////////////////////
// WARNING
// This was originally long double. However, that
// messes up hashing. long double is 80-bit precision,
// but stored as 128-bit, with unused garbage after
// the first 10 bytes. This might be fixable, but
// double is ok for now
///////////////////////////////////////////////////////
//! Floating point options are stored as this type
typedef double OptionFloat;




////////////////////////////////////////////////
// Available option types
////////////////////////////////////////////////
enum class OptionType
{
    Int, Float, Bool, String,
    SetInt, SetFloat, SetBool, SetString,
    ListInt, ListFloat, ListBool, ListString,
    DictIntInt, DictIntFloat, DictIntBool, DictIntString,
    DictStringInt, DictStringFloat, DictStringBool, DictStringString
};



/*! \brief Returns a descriptive string for an OptionType */
const char * OptionTypeToString(OptionType ot);




///////////////////////////////////////
// Information about an OptionType
// This maps an option type to its stored type
///////////////////////////////////////
template<OptionType T> struct OptionTypeInfo { };

// For most options
#define MAKE_OPTION_TYPE(OPTTYPE, STOREDTYPE) \
        template<> struct OptionTypeInfo<OptionType::OPTTYPE> { \
        typedef STOREDTYPE stored_type; };

// for dicts/maps
#define MAKE_OPTION_DICT_TYPE(OPTTYPE, STOREDKEYTYPE, STOREDMAPPEDTYPE) \
        template<> struct OptionTypeInfo<OptionType::OPTTYPE> { \
        typedef std::map<STOREDKEYTYPE, STOREDMAPPEDTYPE> stored_type; };

MAKE_OPTION_TYPE(Int,         OptionInt)
MAKE_OPTION_TYPE(Float,       OptionFloat)
MAKE_OPTION_TYPE(Bool,        bool)
MAKE_OPTION_TYPE(String,      std::string)

MAKE_OPTION_TYPE(ListInt,     std::vector<OptionInt>)
MAKE_OPTION_TYPE(ListFloat,   std::vector<OptionFloat>)
MAKE_OPTION_TYPE(ListBool,    std::vector<bool>)
MAKE_OPTION_TYPE(ListString,  std::vector<std::string>)

MAKE_OPTION_TYPE(SetInt,      std::set<OptionInt>)
MAKE_OPTION_TYPE(SetFloat,    std::set<OptionFloat>)
MAKE_OPTION_TYPE(SetBool,     std::set<bool>)
MAKE_OPTION_TYPE(SetString,   std::set<std::string>);


MAKE_OPTION_DICT_TYPE(DictIntInt,       OptionInt,   OptionInt)
MAKE_OPTION_DICT_TYPE(DictIntFloat,     OptionInt,   OptionFloat)
MAKE_OPTION_DICT_TYPE(DictIntBool,      OptionInt,   bool)
MAKE_OPTION_DICT_TYPE(DictIntString,    OptionInt,   std::string)

MAKE_OPTION_DICT_TYPE(DictStringInt,    std::string, OptionInt)
MAKE_OPTION_DICT_TYPE(DictStringFloat,  std::string, OptionFloat)
MAKE_OPTION_DICT_TYPE(DictStringBool,   std::string, bool)
MAKE_OPTION_DICT_TYPE(DictStringString, std::string, std::string)

#undef MAKE_OPTION_TYPE
#undef MAKE_OPTION_DICT_TYPE


////////////////////////////////////
// Mapping of a single type to its
// List and Set counterparts
////////////////////////////////////
template<OptionType OPTTYPE> struct ContainerMap { };

#define MAKE_CONTAINER_TYPE(OPTTYPE, SETTYPE, LISTTYPE) \
    template<> struct ContainerMap<OptionType::OPTTYPE> { \
       static constexpr OptionType set_type = OptionType::SETTYPE; \
       static constexpr OptionType list_type = OptionType::LISTTYPE; \
    };

MAKE_CONTAINER_TYPE(Int,    SetInt,    ListInt)
MAKE_CONTAINER_TYPE(Float,  SetFloat,  ListFloat)
MAKE_CONTAINER_TYPE(Bool,   SetBool,   ListBool)
MAKE_CONTAINER_TYPE(String, SetString, ListString)

#undef MAKE_CONTAINER_TYPE



//////////////////////////////
// For map/dictionary types
// we need two separate values
//////////////////////////////
template<OptionType KEYTYPE, OptionType VALTYPE> struct ContainerMap_Dict { };

#define MAKE_CONTAINER_TYPE_DICT(KEYTYPE, VALTYPE, DICTTYPE) \
    template<> struct ContainerMap_Dict<KEYTYPE, VALTYPE> { \
       static constexpr OptionType dict_type = DICTTYPE; \
    };

MAKE_CONTAINER_TYPE_DICT(OptionType::Int,    OptionType::Int,    OptionType::DictIntInt) 
MAKE_CONTAINER_TYPE_DICT(OptionType::Int,    OptionType::Float,  OptionType::DictIntFloat) 
MAKE_CONTAINER_TYPE_DICT(OptionType::Int,    OptionType::Bool,   OptionType::DictIntBool) 
MAKE_CONTAINER_TYPE_DICT(OptionType::Int,    OptionType::String, OptionType::DictIntString) 
MAKE_CONTAINER_TYPE_DICT(OptionType::String, OptionType::Int,    OptionType::DictStringInt) 
MAKE_CONTAINER_TYPE_DICT(OptionType::String, OptionType::Float,  OptionType::DictStringFloat) 
MAKE_CONTAINER_TYPE_DICT(OptionType::String, OptionType::Bool,   OptionType::DictStringBool) 
MAKE_CONTAINER_TYPE_DICT(OptionType::String, OptionType::String, OptionType::DictStringString) 

#undef MAKE_CONTAINER_TYPE_DICT



////////////////////////////////////////
// Mapping of POD to their corresponding
// option types
////////////////////////////////////////

template<typename T> struct OptionTypeMap { static constexpr bool valid = false; };

#define MAPTOOPTIONTYPE(TYPE, OPTTYPE) \
        template<> struct OptionTypeMap<TYPE> { \
        static constexpr OptionType opt_type = OptionType::OPTTYPE; };

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
//RMR- Seems silly to change macro to include N, for this scenario
template<size_t N> struct OptionTypeMap<char[N]> { 
static constexpr OptionType opt_type = OptionType::String; };
#undef MAPTOOPTIONTYPE


//////////////////////////
// Containers
//////////////////////////
// Sets

template<typename T>
struct OptionTypeMap<std::set<T>>
{
    static constexpr OptionType inner_type = OptionTypeMap<T>::opt_type;
    static constexpr OptionType opt_type = ContainerMap<inner_type>::set_type;
};

// Lists/vectors
template<typename T>
struct OptionTypeMap<std::vector<T>>
{
    static constexpr OptionType inner_type = OptionTypeMap<T>::opt_type;
    static constexpr OptionType opt_type = ContainerMap<inner_type>::list_type;
};

// Maps
template<typename T, typename U>
struct OptionTypeMap<std::map<T, U>>
{
    static constexpr OptionType inner_key_type = OptionTypeMap<T>::opt_type;
    static constexpr OptionType inner_val_type = OptionTypeMap<U>::opt_type;
    static constexpr OptionType opt_type = ContainerMap_Dict<inner_key_type, inner_val_type>::dict_type;
};


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
    static Target Cast(const Source & s)   {  return math::numeric_cast<Target, Source>(s);  }
};



/*! \brief Converts an option value to/from its stored type
 *
 * Specialization for string types
 */
template<>
struct OptionCast<std::string, std::string>
{
    static std::string Cast(const std::string & s)   {  return s; }
};

/*! \brief Converts an option value to/from its stored type
 *
 * Specialization for string literal types
 */
template<size_t N>
struct OptionCast<std::string, char [N]>
{
    static std::string Cast(const char* s)   {  return s; }
};


/*! \brief Converts vectors of a given type to/from its stored type */
template<typename Target, typename Source>
struct OptionCast<std::vector<Target>, std::vector<Source>>
{
    static std::vector<Target> Cast(const std::vector<Source> & s)
    {
        std::vector<Target> t;
        std::transform(s.begin(), s.end(), std::back_inserter(t),
                       OptionCast<Target, Source>::Cast);
        return t;
    }
};


/*! \brief Converts an option value to/from its stored type
 *
 * Specialization for set types
 */
template<typename Target, typename Source>
struct OptionCast<std::set<Target>, std::set<Source>>
{
    static std::set<Target> Cast(const std::set<Source> & s)
    {
        std::set<Target> t;
        std::transform(s.begin(), s.end(), std::inserter(t),
                       OptionCast<Target, Source>::Cast);
        return t;
    }
};


/*! \brief Converts an option value to/from its stored type
 *
 * Specialization for map types
 */
template<typename K1, typename M1, typename K2, typename M2>
struct OptionCast<std::map<K1, M1>, std::map<K2, M2>>
{
    static std::map<K1, M1> Cast(const std::map<K2, M2> & s)
    {
        std::map<K1, M1> target;
        for(const auto & it : s)
            target.emplace(OptionCast<K1, K2>::Cast(it.first), OptionCast<M1, M2>::Cast(it.second));
        return target;
    }
};



} // close namespace datastore
} // close namespace bpmodule

#endif
