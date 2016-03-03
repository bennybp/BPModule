/*! \file
 *
 * \brief Parallelization functionality (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef BPMODULE_GUARD_PARALLEL__INITFINALIZE_HPP_
#define BPMODULE_GUARD_PARALLEL__INITFINALIZE_HPP_

#include <memory>//For unique ptr

//! \todo We need to store multiple communicators, etc

namespace LibTaskForce{
    class Environment;
}




namespace bpmodule {
namespace parallel {

//For the moment we have a global environment (basically the top-level MPI_COMM)
extern std::unique_ptr<LibTaskForce::Environment> Env_;    
    
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
