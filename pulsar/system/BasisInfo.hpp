/*\file
 *
 * \brief Atoms and atomic centers (header)
*/


#ifndef PULSAR_GUARD_SYSTEM__BASISINFO_HPP_
#define PULSAR_GUARD_SYSTEM__BASISINFO_HPP_

#include "pulsar/system/BasisShellInfo.hpp"
#include "bphash/Hasher.hpp"

#include <string>
#include <vector>

namespace pulsar {
namespace system {

//! Information stored about the basis set on the atom
struct BasisInfo
{
    std::string description;             //!< Description of basis (basis set name, etc)
    std::vector<BasisShellInfo> shells;  //!< Actual basis

    bool operator==(const BasisInfo & rhs) const
    {
        return (shells == rhs.shells &&
                description == rhs.description);
    }

    bool operator!=(const BasisInfo & rhs) const
    {
        return !((*this) == rhs);
    }

    //! \name Serialization and Hashing
    ///@{

    template<class Archive>
    void serialize(Archive & ar)
    {
        ar(description, shells);
    }

    void hash(bphash::Hasher & h) const
    {
        h(description, shells);
    }

    ///@}
};

} // close namespace pulsar
} // close namespace system

#endif

