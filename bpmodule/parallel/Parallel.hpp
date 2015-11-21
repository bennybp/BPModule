/*! \file
 *
 * \brief Parallelization functionality (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef _GUARD_PARALLEL_HPP_
#define _GUARD_PARALLEL_HPP_

#include "bpmodule/python_helper/BoostPython_fwd.hpp"


//! \todo We need to store multiple communicators, etc

namespace bpmodule {
namespace parallel {

/*! \brief Initialize the parallelization functionality
 *
 * This is meant to be called from python
 */
void InitParallel(void);


/*! \brief Finalize parallelization functionality
 *
 * This is meant to be called from python
 */
void FinalizeParallel(void);


/*! \brief Return the ID (rank) associated with this process
 */
long GetProcID(void);


/*! \brief Return the total number of processes
 */
long GetNProc(void);


} // close namespace parallel
} // close namespace bpmodule

#endif
