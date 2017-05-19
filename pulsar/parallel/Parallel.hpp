/*! \file
 *
 * \brief Parallelization functionality (header)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 


#ifndef PULSAR_GUARD_PARALLEL__PARALLEL_HPP_
#define PULSAR_GUARD_PARALLEL__PARALLEL_HPP_

#include <cstddef> //For size_t
#include <functional>
#include <mpi.h>


namespace pulsar {

    
/*! \brief Initialize the parallelization functionality
 *
 * This is meant to be called from python at the start of the program
 * 
 * \param[in] nthreads The maximum number of threads the program may use
 * 
 * 
 * \todo Make an overload that takes an MPI_COMM instance
 */
void parallel_initialize(size_t nthreads);


/*! \brief Finalize parallelization functionality
 *
 * \warning This is never meant to be called from python. Instead,
 *          it is called by the .so file destructor
 */
void parallel_finalize(void);


/*! \brief Return the ID (rank) associated with this process
 */
long get_proc_id(void);


/*! \brief Return the total number of processes
 */
long get_nproc(void);

} // close namespace pulsar

#endif
