
#include <map>
#include "pulsar/datastore/OptionTypes.hpp"

using namespace pulsar;

static std::map<OptionType, const char *> option_namemap_{
          {OptionType::Int,        "Int"},
          {OptionType::Float,      "Float"},
          {OptionType::Bool,       "Bool"},
          {OptionType::String,     "String"},

          {OptionType::ListInt,    "ListInt"},
          {OptionType::ListFloat,  "ListFloat"},
          {OptionType::ListBool,   "ListBool"},
          {OptionType::ListString, "ListString"},

          {OptionType::SetInt,    "SetInt"},
          {OptionType::SetFloat,  "SetFloat"},
          {OptionType::SetBool,   "SetBool"},
          {OptionType::SetString, "SetString"},

          {OptionType::DictIntInt,         "DictIntInt"},
          {OptionType::DictIntFloat,       "DictIntFloat"},
          {OptionType::DictIntBool,        "DictIntBool"},
          {OptionType::DictIntString,      "DictIntString"},
          {OptionType::DictStringInt,      "DictStringInt"},
          {OptionType::DictStringFloat,    "DictStringFloat"},
          {OptionType::DictStringBool,     "DictStringBool"},
          {OptionType::DictStringString,   "DictStringString"}
};




namespace pulsar {

const char * OptionTypeToString(OptionType ot)
{
    if(option_namemap_.count(ot) == 0)
        throw std::logic_error("Undefined OptionType");
    else
        return option_namemap_.at(ot);
}


} // closing namespace pulsar
