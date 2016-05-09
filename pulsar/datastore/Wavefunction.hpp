/*! \file
 *
 * \brief General wavefunction class
 * \author Benjamin Pritchard (ben@bennyp.org)
 */

#ifndef PULSAR_GUARD_DATASTORE__WAVEFUNCTION_HPP_
#define PULSAR_GUARD_DATASTORE__WAVEFUNCTION_HPP_

#include <memory>

#include "pulsar/math/IrrepSpinMatrix.hpp"
#include "pulsar/system/System.hpp"
#include "pulsar/util/Serialization.hpp"
#include "pulsar/util/bphash/Hash.hpp"
#include "pulsar/util/bphash/Hasher_fwd.hpp"

namespace pulsar {
namespace datastore {


/*! \brief Stores the state of a system
 *
 * \par Hashing
 * The hash of a wavefunction depends on the hash of
 * its component parts. See hashing of System and IrrepSpinMatrices
 */
class Wavefunction
{
    public:
        bool HasSystem(void) const noexcept
        {
            return static_cast<bool>(system.first);
        }

        std::shared_ptr<const system::System> GetSystem(void) const
        {
            return system.first;
        }

        util::Hash GetSystemHash(void) const
        {
            return system.second;
        }

        void SetSystem(std::shared_ptr<const system::System> sys)
        {
            system = {sys, sys ? sys->MyHash() : util::Hash()};
        }

        void SetSystem(const system::System & sys)
        {
            SetSystem(std::make_shared<const system::System>(sys));
        }

        void SetSystem(system::System && sys)
        {
            SetSystem(std::make_shared<const system::System>(std::move(sys)));
        }

        bool HasCMat(void) const noexcept
        {
            return static_cast<bool>(cmat.first);
        }

        std::shared_ptr<const math::IrrepSpinMatrixD> GetCMat(void) const
        {
            return cmat.first;
        }

        util::Hash GetCMatHash(void) const
        {
            return cmat.second;
        }

        void SetCMat(std::shared_ptr<const math::IrrepSpinMatrixD> c)
        {
            cmat = {c, c ? c->MyHash() : util::Hash()};
        }

        void SetCMat(const math::IrrepSpinMatrixD & c)
        {
            SetCMat(std::make_shared<const math::IrrepSpinMatrixD>(c));
        }

        void SetCMat(math::IrrepSpinMatrixD && c)
        {
            SetCMat(std::make_shared<const math::IrrepSpinMatrixD>(std::move(c)));
        }

        bool HasEpsilon(void) const noexcept
        {
            return static_cast<bool>(epsilon.first);
        }

        std::shared_ptr<const math::IrrepSpinVectorD> GetEpsilon(void) const
        {
            return epsilon.first;
        }

        util::Hash GetEpsilonHash(void) const
        {
            return epsilon.second;
        }

        void SetEpsilon(std::shared_ptr<const math::IrrepSpinVectorD> e)
        {
            epsilon = {e, e ? e->MyHash() : util::Hash()};
        }

        void SetEpsilon(const math::IrrepSpinVectorD & e)
        {
            SetEpsilon(std::make_shared<const math::IrrepSpinVectorD>(e));
        }

        void SetEpsilon(math::IrrepSpinVectorD && e)
        {
            SetEpsilon(std::make_shared<const math::IrrepSpinVectorD>(std::move(e)));
        }

        bool HasOccupations(void) const noexcept
        {
            return static_cast<bool>(occupations.first);
        }

        std::shared_ptr<const math::IrrepSpinVectorD> GetOccupations(void) const
        {
            return occupations.first;
        }

        util::Hash GetOccupationsHash(void) const
        {
            return occupations.second;
        }

        void SetOccupations(std::shared_ptr<const math::IrrepSpinVectorD> occ)
        {
            occupations = {occ, occ ? occ->MyHash() : util::Hash()};
        }

        void SetOccupations(const math::IrrepSpinVectorD & occ)
        {
            SetOccupations(std::make_shared<const math::IrrepSpinVectorD>(occ));
        }

        void SetOccupations(math::IrrepSpinVectorD && occ)
        {
            SetOccupations(std::make_shared<const math::IrrepSpinVectorD>(std::move(occ)));
        }

        void ValidCheck(void) const
        {
            using exception::GeneralException;

            // check for consistent irreps, spins, etc,
            // accross all members 
            //! \todo compare with system?
            if(cmat.first)
            {
                if(epsilon.first && !cmat.first->SameStructure(*epsilon.first))
                    throw GeneralException("Inconsistent shape: epsilon and cmat");
                if(occupations.first && !cmat.first->SameStructure(*occupations.first))
                    throw GeneralException("Inconsistent shape: occupations and cmat");
            }

            if(epsilon.first)
            {
                if(occupations.first && !epsilon.first->SameStructure(*occupations.first))
                    throw GeneralException("Inconsistent shape: occupations and epsilon");
            }
        }


        bool operator==(const Wavefunction & rhs) const;

        bool operator!=(const Wavefunction & rhs) const;

        util::Hash MyHash(void) const;

    private:
        std::pair<std::shared_ptr<const system::System>,         util::Hash> system;
        std::pair<std::shared_ptr<const math::IrrepSpinMatrixD>, util::Hash> cmat;
        std::pair<std::shared_ptr<const math::IrrepSpinVectorD>, util::Hash> epsilon;
        std::pair<std::shared_ptr<const math::IrrepSpinVectorD>, util::Hash> occupations;

        //! \name Serialization and Hashing
        ///@{

        DECLARE_SERIALIZATION_FRIENDS
        DECLARE_HASHING_FRIENDS

        /* We have to split load/save since the
         * the shared_ptr points to const data, and
         * cereal can't serialize to const data
         */
        template<class Archive>
        void save(Archive & ar) const
        {
            ar(system.first, cmat.first, epsilon.first, occupations.first);
        }

        template<class Archive>
        void load(Archive & ar)
        {
            std::shared_ptr<system::System> newsystem;
            std::shared_ptr<math::IrrepSpinMatrixD> newcmat;
            std::shared_ptr<math::IrrepSpinVectorD> newepsilon;
            std::shared_ptr<math::IrrepSpinVectorD> newocc;

            ar(newsystem, newcmat, newepsilon, newocc);

            SetSystem(newsystem);
            SetCMat(newcmat);
            SetEpsilon(newepsilon);
            SetOccupations(newocc);
        }

        void hash(util::Hasher & h) const;

        ///@}
};


} // close namespace datastore
} // close namespace system

#endif
