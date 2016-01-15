/*! \file
 *
 * \brief Parallelization functionality (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef BPMODULE_GUARD_PARALLEL__INITFINALIZE_HPP_
#define BPMODULE_GUARD_PARALLEL__INITFINALIZE_HPP_


//! \todo We need to store multiple communicators, etc

namespace bpmodule {
namespace parallel {

/*! \brief Initialize the parallelization functionality
 *
 * This is meant to be called from python
 */
void Init(void);


/*! \brief Finalize parallelization functionality
 *
 * This is meant to be called from python
 */
void Finalize(void);


/*! \brief Return the ID (rank) associated with this process
 */
long GetProcID(void);


/*! \brief Return the total number of processes
 */
long GetNProc(void);


} // close namespace parallel
} // close namespace bpmodule

#endif
