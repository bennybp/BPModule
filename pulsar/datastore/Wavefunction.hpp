/*! \file
 *
 * \brief General wavefunction class
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#ifndef PULSAR_GUARD_DATASTORE__WAVEFUNCTION_HPP_
#define PULSAR_GUARD_DATASTORE__WAVEFUNCTION_HPP_

#include <memory>

#include "pulsar/exception/Exceptions.hpp"
#include "pulsar/math/IrrepSpinMatrix.hpp"
#include "pulsar/system/System.hpp"
#include "pulsar/util/HashSerializable.hpp"
#include "pulsar/util/Serialization.hpp"


namespace pulsar{
namespace datastore {

class Wavefunction
{
    public:
        std::shared_ptr<const system::System> system;
        std::shared_ptr<const math::IrrepSpinMatrixD> cmat;
        std::shared_ptr<const math::IrrepSpinVectorD> epsilon;

        util::Hash MyHash(void) const
        {
            return util::HashSerializable(*this);
        }

        bool operator==(const Wavefunction & rhs) const
        {
            // check for pointer equivalence first
            bool samesystem = (system == rhs.system);
            bool samecmat = (cmat == rhs.cmat);
            bool sameepsilon = (epsilon == rhs.epsilon);

            // if the pointers aren't the same, check the 
            // actual data
            if(!samesystem && bool(system) && bool(rhs.system))
                samesystem = ( (*system) == (*rhs.system) );

            if(!samecmat && bool(cmat) && bool(rhs.cmat))
                samecmat = ( (*cmat) == (*rhs.cmat) );

            if(!sameepsilon && bool(epsilon) && bool(rhs.epsilon))
                sameepsilon = ( (*epsilon) == (*rhs.epsilon) );
                    
            return (samesystem && samecmat && sameepsilon);
        }


    private:
        //! \name Serialization
        ///@{

        DECLARE_SERIALIZATION_FRIENDS

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

        ///@}
};


} // close namespace datastore
} // close namespace system

#endif
