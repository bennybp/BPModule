#pragma once

#include "pulsar/modulebase/ModuleBase.hpp"
#include "pulsar/system/BasisSet.hpp"

namespace pulsar{

/*! \brief Rank3 Tensor builder implementation
 *
 */
class Rank3Builder : public ModuleBase
{
    public:
        typedef Rank3Builder BaseType;
        typedef std::string HashType;
        typedef std::vector<std::shared_ptr<const Rank3DImpl>> ReturnType;

        Rank3Builder(ID_t id)
            : ModuleBase(id, "Rank3Builder")
        { }


        ReturnType calculate(const std::string & key,
                             unsigned int deriv, const Wavefunction & wfn,
                             const BasisSet& bs1,
                             const BasisSet& bs2,
                             const BasisSet& bs3)
        {
            auto hash=my_hash(key,deriv,wfn,bs1,bs2,bs3);
            auto cache_value=cache().get<ReturnType>(hash,false);
            if(cache_value)return *cache_value;
            auto rv = ModuleBase::call_function(&Rank3Builder::calculate_,
                                                key, deriv, wfn, bs1, bs2,bs3);
            cache().set(hash,rv,CacheMap::CachePolicy::CheckpointLocal);
            return rv;
        }

        HashType my_hash(const std::string & key,
                         unsigned int deriv,
                         const Wavefunction & wfn,
                         const BasisSet& bs1,
                         const BasisSet& bs2,
                         const BasisSet& bs3)
        {
            return ModuleBase::call_function(&Rank3Builder::my_hash_,
                                                key, deriv, wfn, bs1, bs2,bs3);
        }


        /////////////////////////////////////////
        // To be implemented by derived classes
        /////////////////////////////////////////

        //! \copydoc calculate
        virtual ReturnType calculate_(const std::string & key,
                                      unsigned int deriv, const Wavefunction & wfn,
                                      const BasisSet & bs1, const BasisSet & bs2,
                                      const BasisSet & bs3) = 0;

        virtual HashType my_hash_(const std::string & key,
                                  unsigned int deriv, const Wavefunction & wfn,
                                  const BasisSet & bs1, const BasisSet & bs2,
                                  const BasisSet & bs3)=0;

};


class Rank3Builder_Py : public Rank3Builder
{
    public:
        using Rank3Builder::Rank3Builder;

        MODULEBASE_FORWARD_PROTECTED_TO_PY

        virtual ReturnType calculate_(const std::string & key,
                                      unsigned int deriv, const Wavefunction & wfn,
                                      const BasisSet & bs1, const BasisSet & bs2,
                                      const BasisSet & bs3)
        {
            return call_py_override<ReturnType>(this, "calculate_", key, deriv, wfn, bs1, bs2,bs3);
        }

        virtual HashType my_hash_(const std::string & key,
                                      unsigned int deriv, const Wavefunction & wfn,
                                      const BasisSet & bs1, const BasisSet & bs2,
                                      const BasisSet & bs3)
        {
            return call_py_override<HashType>(this, "my_hash_", key, deriv, wfn, bs1, bs2,bs3);
        }
};

} // close namespace pulsar
