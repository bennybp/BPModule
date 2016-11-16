/*! \file
 *
 * \brief Parallelization functionality (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef PULSAR_GUARD_PARALLEL__PARALLEL_HPP_
#define PULSAR_GUARD_PARALLEL__PARALLEL_HPP_

#include <cstddef> //For size_t
#include <functional>
#include <LibTaskForce/LibTaskForce.hpp>


namespace pulsar {

//Returns the current environment
const LibTaskForce::HybridEnv& get_env();    
    
/*! \brief Initialize the parallelization functionality
 *
 * This is meant to be called from python at the start of the program
 * 
 * \param[in] NThreads The maximum number of threads the program may use
 * 
 * 
 * \todo Make an overload that takes an MPI_COMM instance
 */
void initialize(size_t NThreads);


/*! \brief Finalize parallelization functionality
 *
 * This is meant to be called from python
 */
void finalize(void);


/*! \brief Return the ID (rank) associated with this process
 */
long get_proc_id(void);


/*! \brief Return the total number of processes
 */
long get_nproc(void);

} // close namespace pulsar

#endif
