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
#include "bphash/Hasher.hpp"

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

        void valid_check(void) const
        {
            using exception::GeneralException;

            // check for consistent irreps, spins, etc,
            // accross all members 
            //! \todo compare with system?
            if(cmat)
            {
                if(opdm && !cmat->same_structure(*opdm))
                    throw GeneralException("Inconsistent shape: opdm and cmat");
                if(epsilon && !cmat->same_structure(*epsilon))
                    throw GeneralException("Inconsistent shape: epsilon and cmat");
                if(occupations && !cmat->same_structure(*occupations))
                    throw GeneralException("Inconsistent shape: occupations and cmat");
            }

            if(opdm)
            {
                if(epsilon && !opdm->same_structure(*epsilon))
                    throw GeneralException("Inconsistent shape: epsilon and opdm");
                if(occupations && !opdm->same_structure(*occupations))
                    throw GeneralException("Inconsistent shape: occupations and opdm");
            }

            if(epsilon)
            {
                if(occupations && !epsilon->same_structure(*occupations))
                    throw GeneralException("Inconsistent shape: occupations and epsilon");
            }
        }


        bool operator==(const Wavefunction & rhs) const;

        bool operator!=(const Wavefunction & rhs) const;

        bphash::HashValue my_hash(void) const;

    private:

        //! \name Serialization and Hashing
        ///@{

        DECLARE_SERIALIZATION_FRIENDS
        friend class bphash::Hasher;

        /* We have to split load/save since the
         * the shared_ptr points to const data, and
         * cereal can't serialize to const data
         */
        template<class Archive>
        void save(Archive & ar) const
        {
            throw exception::GeneralException("TODO - serialization of wfn");
            //ar(system, cmat, epsilon, occupations);
        }

        template<class Archive>
        void load(Archive & ar)
        {
            throw exception::GeneralException("TODO - serialization of wfn");
            /*
            std::shared_ptr<system::System> newsystem;
            std::shared_ptr<math::IrrepSpinMatrixD> newcmat;
            std::shared_ptr<math::IrrepSpinVectorD> newepsilon;
            std::shared_ptr<math::IrrepSpinVectorD> newocc;

            ar(newsystem, newcmat, newepsilon, newocc);

            system = newsystem; 
            cmat = newcmat;
            epsilon = newepsilon;
            occupations = newocc;
            */
        }

        void hash(bphash::Hasher & h) const;

        ///@}
};


} // close namespace datastore
} // close namespace system

#endif
