/*! \file
 *
 * \brief Parallelization functionality (source)
 * \author Benjamin Pritchard (ben@bennyp.org)
 */ 

#include <iostream>
#include <memory>
#include <mpi.h>
#include "pulsar/exception/PulsarException.hpp"
#include "pulsar/parallel/Parallel.hpp"

namespace {

    /*! \brief This class adds RAII semantics to the MPI environment
     *
     * On destruction, an object of this class will clean up the
     * MPI environment and call MPI_Finalize. It will only do this
     * if it was responsible for calling MPI_Init, however.
     */
    struct EnvManager_
    {
        bool initialized_by_me = false;

        void initialize(size_t )
        {
            using namespace pulsar;

            // is MPI already initialized?
            int initialized;
            MPI_Initialized(&initialized);

            if(!initialized)
            {
                int provided;
                MPI_Init_thread(nullptr, nullptr,
                                MPI_THREAD_MULTIPLE, &provided);

                if(provided!=MPI_THREAD_MULTIPLE)
                    throw PulsarException("MPI does not support threading");

                initialized_by_me = true;
            }

        }

        int rank(void)const
        {
            int my_rank;
            MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
            return my_rank;
        }

        int n_procs(void)const
        {
            int my_size;
            MPI_Comm_size(MPI_COMM_WORLD,&my_size);
            return my_size;
        }

        void finalize(void)
        {

            // is MPI already initialized?
            int initialized;
            MPI_Initialized(&initialized);
            if(initialized && initialized_by_me)
            {
                initialized_by_me = false;
                MPI_Finalize();
            }
        }
        
        ~EnvManager_()
        {
            finalize();
        }
    };
  
    //! Global object for managing the MPI environment in Pulsar 
    EnvManager_ envmanager_; 

} // close anonymous namespace

namespace pulsar{


void parallel_initialize(size_t nthreads)
{
    envmanager_.initialize(nthreads);
    std::cout << "Initialized process " << get_proc_id()
              << " of " << get_nproc() << "\n";
}


long get_proc_id(void)
{
    return static_cast<long>(envmanager_.rank());
}


long get_nproc(void)
{
    return static_cast<long>(envmanager_.n_procs());
}

} // close namespace pulsar
