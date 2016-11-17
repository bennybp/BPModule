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

/*! \brief Stores the state of a system
 *
 * \par Hashing
 * The hash of a wavefunction depends on the hash of
 * its component parts. See hashing of System and IrrepSpinMatrices
 */
class Wavefunction
{
    public:
        std::shared_ptr<const System> system;
        std::shared_ptr<const IrrepSpinMatrixD> cmat;        //!< MO Coefficient Matrix
        std::shared_ptr<const IrrepSpinMatrixD> opdm;        //!< One-particle density matrix
        std::shared_ptr<const IrrepSpinVectorD> epsilon;     //!< Orbital energies
        std::shared_ptr<const IrrepSpinVectorD> occupations; //!< Occupation of MOs

        void valid_check(void) const
        {
            // check for consistent irreps, spins, etc,
            // accross all members 
            //! \todo compare with system?
            if(cmat)
            {
                if(opdm && !cmat->same_structure(*opdm))
                    throw PulsarException("Inconsistent shape: opdm and cmat");
                if(epsilon && !cmat->same_structure(*epsilon))
                    throw PulsarException("Inconsistent shape: epsilon and cmat");
                if(occupations && !cmat->same_structure(*occupations))
                    throw PulsarException("Inconsistent shape: occupations and cmat");
            }

            if(opdm)
            {
                if(epsilon && !opdm->same_structure(*epsilon))
                    throw PulsarException("Inconsistent shape: epsilon and opdm");
                if(occupations && !opdm->same_structure(*occupations))
                    throw PulsarException("Inconsistent shape: occupations and opdm");
            }

            if(epsilon)
            {
                if(occupations && !epsilon->same_structure(*occupations))
                    throw PulsarException("Inconsistent shape: occupations and epsilon");
            }
        }


        bool operator==(const Wavefunction & rhs) const;

        bool operator!=(const Wavefunction & rhs) const;

        bphash::HashValue my_hash(void) const;

    private:

        //! \name Serialization and Hashing
        ///@{

        DECLARE_SERIALIZATION_FRIENDS
        BPHASH_DECLARE_HASHING_FRIENDS

        /* We have to split load/save since the
         * the shared_ptr points to const data, and
         * cereal can't serialize to const data
         */
        template<class Archive>
        void save(Archive & ar) const
        {
            ar(system, cmat, epsilon, occupations, opdm);
        }

        template<class Archive>
        void load(Archive & ar)
        {
            std::shared_ptr<System> newsystem;
            std::shared_ptr<IrrepSpinMatrixD> newcmat;
            std::shared_ptr<IrrepSpinVectorD> newepsilon;
            std::shared_ptr<IrrepSpinMatrixD> newopdm;
            std::shared_ptr<IrrepSpinVectorD> newocc;

            ar(newsystem, newcmat, newepsilon, newocc, newopdm);

            system = newsystem; 
            cmat = newcmat;
            epsilon = newepsilon;
            occupations = newocc;
            opdm = newopdm;

        }

        void hash(bphash::Hasher & h) const;

        ///@}
};

} // close namespace system

#endif
