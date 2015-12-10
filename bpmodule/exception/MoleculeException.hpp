/*! \file
 *
 * \brief An exception for molecules and atoms
 * \author Benjamin Pritchard (ben@bennyp.org)
 */


#ifndef _GUARD_MOLECULEEXCEPTION_HPP_
#define _GUARD_MOLECULEEXCEPTION_HPP_

#include "bpmodule/exception/GeneralException.hpp"

namespace bpmodule {
namespace exception {


/*! \brief An exception thrown when there is a problem with molecules and atoms
 *
 * Some exceptions may include poorly-defined molecules, symmetry problems,
 * or problems looking up atomic information.
 */
class MoleculeException : public GeneralException
{
    public:
        /*! \brief Constructor
         *
         * \param [in] whatstr Brief description of the error
         * \param [in] exinfo Additional information. Must be an even number of strings
         */
        template<typename... Targs>
        MoleculeException(const std::string & whatstr,
                        const Targs&... exinfo)

            : GeneralException(whatstr, exinfo...)
        { }


        /*! \brief Constructor, using another exception as a base
         *
         * Can be used to append additional information
         *
         * \param [in] gex Exception to copy
         * \param [in] exinfo Additional information. Must be an even number of strings
         */
        template<typename... Targs>
        MoleculeException(const GeneralException & gex, const Targs&... exinfo)
            : GeneralException(gex, exinfo...)
        { }


        MoleculeException()                                      = delete;
        MoleculeException(const MoleculeException & rhs)             = default;
        MoleculeException(MoleculeException && rhs)                  = default;
        MoleculeException & operator=(const MoleculeException & rhs) = default;
        MoleculeException & operator=(MoleculeException && rhs)      = default;
        virtual ~MoleculeException() = default;
};



}// close namespace exception
}// close namespace bpmodule



#endif
