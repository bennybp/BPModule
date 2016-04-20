/*! \file
 *
 * \brief Parallelization functionality (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef PULSAR_GUARD_PARALLEL__INITFINALIZE_HPP_
#define PULSAR_GUARD_PARALLEL__INITFINALIZE_HPP_

#include <cstddef> //For size_t

//! \todo We need to store multiple communicators, etc

namespace LibTaskForce{
    class Environment;
}

namespace pulsar{
namespace parallel {

//Returns the current environment
const LibTaskForce::Environment& GetEnv();    
    
/*! \brief Initialize the parallelization functionality
 *
 * This is meant to be called from python at the start of the program
 * 
 * \param[in] NThreads The maximum number of threads the program may use
 * 
 * 
 * \todo Make an overload that takes an MPI_COMM instance
 */
void Init(size_t NThreads);


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
} // close namespace pulsar

#endif
