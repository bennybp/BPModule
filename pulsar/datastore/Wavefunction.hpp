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


namespace pulsar{
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
        std::shared_ptr<const math::IrrepSpinMatrixD> cmat;
        std::shared_ptr<const math::IrrepSpinVectorD> epsilon;

        /*! \brief Return a unique hash for this wavefunction */
        util::Hash MyHash(void) const;

        bool operator==(const Wavefunction & rhs) const;


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
            ar(system, cmat, epsilon);
        }

        template<class Archive>
        void load(Archive & ar)
        {
            std::shared_ptr<system::System> newsystem;
            std::shared_ptr<math::IrrepSpinMatrixD> newcmat;
            std::shared_ptr<math::IrrepSpinVectorD> newepsilon;

            ar(newsystem, newcmat, newepsilon);

            system = std::move(newsystem);
            cmat = std::move(newcmat);
            epsilon = std::move(newepsilon);
        }

        void hash(util::Hasher & h) const;

        ///@}
};


} // close namespace datastore
} // close namespace system

#endif
