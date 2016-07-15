/*! \file
 *
 * \brief Registration of options serialization
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#include "pulsar/datastore/OptionHolder.hpp"
#include "bphash/types/string.hpp"
#include "bphash/types/vector.hpp"
#include "bphash/types/set.hpp"
#include "bphash/types/map.hpp"
#include "bphash/types/memory.hpp"

#include <cereal/archives/binary.hpp>
#include <cereal/types/polymorphic.hpp>

using namespace pulsar::datastore::detail;
using pulsar::datastore::OptionType;

CEREAL_REGISTER_TYPE(OptionHolder<OptionType::Int>);
CEREAL_REGISTER_TYPE(OptionHolder<OptionType::Float>);
CEREAL_REGISTER_TYPE(OptionHolder<OptionType::Bool>);
CEREAL_REGISTER_TYPE(OptionHolder<OptionType::String>);

CEREAL_REGISTER_TYPE(OptionHolder<OptionType::ListInt>);
CEREAL_REGISTER_TYPE(OptionHolder<OptionType::ListFloat>);
CEREAL_REGISTER_TYPE(OptionHolder<OptionType::ListBool>);
CEREAL_REGISTER_TYPE(OptionHolder<OptionType::ListString>);

CEREAL_REGISTER_TYPE(OptionHolder<OptionType::SetInt>);
CEREAL_REGISTER_TYPE(OptionHolder<OptionType::SetFloat>);
CEREAL_REGISTER_TYPE(OptionHolder<OptionType::SetBool>);
CEREAL_REGISTER_TYPE(OptionHolder<OptionType::SetString>);

CEREAL_REGISTER_TYPE(OptionHolder<OptionType::DictIntInt>);
CEREAL_REGISTER_TYPE(OptionHolder<OptionType::DictIntFloat>);
CEREAL_REGISTER_TYPE(OptionHolder<OptionType::DictIntBool>);
CEREAL_REGISTER_TYPE(OptionHolder<OptionType::DictIntString>);

CEREAL_REGISTER_TYPE(OptionHolder<OptionType::DictStringInt>);
CEREAL_REGISTER_TYPE(OptionHolder<OptionType::DictStringFloat>);
CEREAL_REGISTER_TYPE(OptionHolder<OptionType::DictStringBool>);
CEREAL_REGISTER_TYPE(OptionHolder<OptionType::DictStringString>);


