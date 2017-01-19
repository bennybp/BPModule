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
        std::unique_ptr<LibTaskForce::HybridEnv> env;
        bool initialized_by_me = false;

        void initialize(size_t nthreads)
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

            env = std::make_unique<LibTaskForce::HybridEnv>(MPI_COMM_WORLD, nthreads);
        }

        void finalize(void)
        {
            env.reset();

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


const LibTaskForce::HybridEnv & get_env(void)
{
    if(!envmanager_.env)
        throw pulsar::PulsarException("Parallel environment not initialized");
    return *(envmanager_.env);
}
 

void parallel_initialize(size_t nthreads)
{
    envmanager_.initialize(nthreads);
    std::cout << "Initialized process " << get_proc_id()
              << " of " << get_nproc() << "\n";
}


long get_proc_id(void)
{
    return static_cast<long>(get_env().comm().rank());
}


long get_nproc(void)
{
    return static_cast<long>(get_env().comm().nprocs());
}

} // close namespace pulsar
