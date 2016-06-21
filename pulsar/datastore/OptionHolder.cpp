/*! \file
 *
 * \brief Storage of options data (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "pulsar/datastore/OptionHolder.hpp"
#include "pulsar/python/Call.hpp"
#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/output/Output.hpp"

#include "bphash/types/string.hpp"
#include "bphash/types/vector.hpp"
#include "bphash/types/set.hpp"
#include "bphash/types/map.hpp"

using namespace pulsar::python;
using namespace pulsar::exception;
using namespace pulsar::util;


namespace pulsar{
namespace datastore {
namespace detail {


///////////////////////////////////////////////////
// These are the allowed types of OptionHolder
///////////////////////////////////////////////////
template class OptionHolder<OptionType::Int>;
template class OptionHolder<OptionType::Float>;
template class OptionHolder<OptionType::Bool>;
template class OptionHolder<OptionType::String>;

template class OptionHolder<OptionType::ListInt>;
template class OptionHolder<OptionType::ListFloat>;
template class OptionHolder<OptionType::ListBool>;
template class OptionHolder<OptionType::ListString>;

template class OptionHolder<OptionType::SetInt>;
template class OptionHolder<OptionType::SetFloat>;
template class OptionHolder<OptionType::SetBool>;
template class OptionHolder<OptionType::SetString>;

template class OptionHolder<OptionType::DictIntInt>;
template class OptionHolder<OptionType::DictIntFloat>;
template class OptionHolder<OptionType::DictIntBool>;
template class OptionHolder<OptionType::DictIntString>;

template class OptionHolder<OptionType::DictStringInt>;
template class OptionHolder<OptionType::DictStringFloat>;
template class OptionHolder<OptionType::DictStringBool>;
template class OptionHolder<OptionType::DictStringString>;



//! \todo make_unique in c++14
#define CASE_RETURN_OPTIONHOLDER(TYPE) \
    case OptionType::TYPE: \
         if(python::is_none(def)) \
             return OptionBasePtr(new OptionHolder<OptionType::TYPE>(key, required, validator, help));\
         else\
             return OptionBasePtr(new OptionHolder<OptionType::TYPE>(key, required, validator, help, def));


OptionBasePtr
CreateOptionHolder(std::string key, OptionType opttype, bool required,
                   const pybind11::object & validator, std::string help,
                   const pybind11::object & def)
{
    switch(opttype)
    {
        CASE_RETURN_OPTIONHOLDER(Int)
        CASE_RETURN_OPTIONHOLDER(Float)
        CASE_RETURN_OPTIONHOLDER(Bool)
        CASE_RETURN_OPTIONHOLDER(String)
        CASE_RETURN_OPTIONHOLDER(SetInt)
        CASE_RETURN_OPTIONHOLDER(SetFloat)
        CASE_RETURN_OPTIONHOLDER(SetBool)
        CASE_RETURN_OPTIONHOLDER(SetString)
        CASE_RETURN_OPTIONHOLDER(ListInt)
        CASE_RETURN_OPTIONHOLDER(ListFloat)
        CASE_RETURN_OPTIONHOLDER(ListBool)
        CASE_RETURN_OPTIONHOLDER(ListString)
        CASE_RETURN_OPTIONHOLDER(DictIntInt)
        CASE_RETURN_OPTIONHOLDER(DictIntFloat)
        CASE_RETURN_OPTIONHOLDER(DictIntBool)
        CASE_RETURN_OPTIONHOLDER(DictIntString)
        CASE_RETURN_OPTIONHOLDER(DictStringInt)
        CASE_RETURN_OPTIONHOLDER(DictStringFloat)
        CASE_RETURN_OPTIONHOLDER(DictStringBool)
        CASE_RETURN_OPTIONHOLDER(DictStringString)
        default:
            throw std::logic_error("Unhandled option type");
    }
}

#undef CASE_RETURN_OPTIONHOLDER


//! \todo make_unique in c++14
#define CASE_UNSERIALIZE_OPTIONHOLDER(TYPE) \
    case OptionType::TYPE: {\
        std::unique_ptr<OptionHolder<OptionType::TYPE>::stored_type> value, def; \
        mar.unserialize(key, required, help, value, def); \
        return OptionBasePtr(new OptionHolder<OptionType::TYPE>\
                    (std::move(key), required, std::move(help), std::move(value), std::move(def)));\
    }

OptionBasePtr
OptionHolderFromByteArray(OptionType opttype, const ByteArray & ba)
{
    MemoryArchive mar;
    mar.from_byte_array(ba);
    std::string key, help;
    bool required;

    mar.begin_unserialization();

    switch(opttype)
    {
        CASE_UNSERIALIZE_OPTIONHOLDER(Int)
        CASE_UNSERIALIZE_OPTIONHOLDER(Float)
        CASE_UNSERIALIZE_OPTIONHOLDER(Bool)
        CASE_UNSERIALIZE_OPTIONHOLDER(String)
        CASE_UNSERIALIZE_OPTIONHOLDER(SetInt)
        CASE_UNSERIALIZE_OPTIONHOLDER(SetFloat)
        CASE_UNSERIALIZE_OPTIONHOLDER(SetBool)
        CASE_UNSERIALIZE_OPTIONHOLDER(SetString)
        CASE_UNSERIALIZE_OPTIONHOLDER(ListInt)
        CASE_UNSERIALIZE_OPTIONHOLDER(ListFloat)
        CASE_UNSERIALIZE_OPTIONHOLDER(ListBool)
        CASE_UNSERIALIZE_OPTIONHOLDER(ListString)
        CASE_UNSERIALIZE_OPTIONHOLDER(DictIntInt)
        CASE_UNSERIALIZE_OPTIONHOLDER(DictIntFloat)
        CASE_UNSERIALIZE_OPTIONHOLDER(DictIntBool)
        CASE_UNSERIALIZE_OPTIONHOLDER(DictIntString)
        CASE_UNSERIALIZE_OPTIONHOLDER(DictStringInt)
        CASE_UNSERIALIZE_OPTIONHOLDER(DictStringFloat)
        CASE_UNSERIALIZE_OPTIONHOLDER(DictStringBool)
        CASE_UNSERIALIZE_OPTIONHOLDER(DictStringString)
        default:
            throw std::logic_error("Unhandled option type");
    }
}

#undef CASE_UNSERIALIZE_OPTIONHOLDER


} // close namespace detail
} // close namespace datastore
} // close namespace pulsar

