##################################################
# Actual library
##################################################

set(BPMODULE_MPI_FILES
            MPI.cpp
            InitMPI.cpp
            FinalizeMPI.cpp
   )


ADD_CORELIB(bpmodule_mpi "${BPMODULE_MPI_FILES}"
                         ""
                         "${Boost_INCLUDE_DIRS};${PYTHON_INCLUDE_DIRS}")

