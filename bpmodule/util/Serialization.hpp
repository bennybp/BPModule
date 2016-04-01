/*! \file
 *
 * \brief Serialization helpers
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef BPMODULE_GUARD_UTIL__SERIALIZATION_HPP_
#define BPMODULE_GUARD_UTIL__SERIALIZATION_HPP_

// Included via SerializationArchives.hpp
//#include <cereal/cereal.hpp>
//#include <cereal/access.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/string.hpp>

#include "bpmodule/util/SerializationArchives.hpp"

#define DECLARE_SERIALIZATION_FRIENDS \
    friend class cereal::access; \
    template<typename T> friend class bpmodule::util::detail::StdStreamArchive;



#endif

