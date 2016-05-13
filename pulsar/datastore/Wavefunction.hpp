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
        std::shared_ptr<const system::System> system;
        std::shared_ptr<const math::IrrepSpinMatrixD> cmat;        //!< MO Coefficient Matrix
        std::shared_ptr<const math::IrrepSpinMatrixD> opdm;        //!< One-particle density matrix
        std::shared_ptr<const math::IrrepSpinVectorD> epsilon;     //!< Orbital energies
        std::shared_ptr<const math::IrrepSpinVectorD> occupations; //!< Occupation of MOs

        void ValidCheck(void) const
        {
            using exception::GeneralException;

            // check for consistent irreps, spins, etc,
            // accross all members 
            //! \todo compare with system?
            if(cmat)
            {
                if(opdm && !cmat->SameStructure(*opdm))
                    throw GeneralException("Inconsistent shape: opdm and cmat");
                if(epsilon && !cmat->SameStructure(*epsilon))
                    throw GeneralException("Inconsistent shape: epsilon and cmat");
                if(occupations && !cmat->SameStructure(*occupations))
                    throw GeneralException("Inconsistent shape: occupations and cmat");
            }

            if(opdm)
            {
                if(epsilon && !opdm->SameStructure(*epsilon))
                    throw GeneralException("Inconsistent shape: epsilon and opdm");
                if(occupations && !opdm->SameStructure(*occupations))
                    throw GeneralException("Inconsistent shape: occupations and opdm");
            }

            if(epsilon)
            {
                if(occupations && !epsilon->SameStructure(*occupations))
                    throw GeneralException("Inconsistent shape: occupations and epsilon");
            }
        }


        bool operator==(const Wavefunction & rhs) const;

        bool operator!=(const Wavefunction & rhs) const;

        util::Hash MyHash(void) const;

    private:

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
            ar(system, cmat, epsilon, occupations);
        }

        template<class Archive>
        void load(Archive & ar)
        {
            std::shared_ptr<system::System> newsystem;
            std::shared_ptr<math::IrrepSpinMatrixD> newcmat;
            std::shared_ptr<math::IrrepSpinVectorD> newepsilon;
            std::shared_ptr<math::IrrepSpinVectorD> newocc;

            ar(newsystem, newcmat, newepsilon, newocc);

            system = newsystem; 
            cmat = newcmat;
            epsilon = newepsilon;
            occupations = newocc;
        }

        void hash(util::Hasher & h) const;

        ///@}
};


} // close namespace datastore
} // close namespace system

#endif
