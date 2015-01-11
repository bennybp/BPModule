#include <iostream>
#include <cstring>
#include <mpi.h>

extern "C" {

void Init(int argc, char ** argv)
{
    // copy argv
    // argv[argc] should always be NULL
    // see http://www.open-mpi.org/community/lists/users/2013/11/22955.php
    char ** argvtmp = new char*[argc+1];
    for(int i = 0; i < argc; i++)
    {
        size_t len = strlen(argv[i]);
        argvtmp[i] = new char[len+1];
        strncpy(argvtmp[i], argv[i], len+1);
    }
    argvtmp[argc] = NULL;
  
    std::cout << "BEFORE\n"; 
    std::cout << "Argc: " << argc << "\n";
    for(int i = 0; i < argc; i++)
        std::cout << i << " : " << argvtmp[i] << "\n";

    std::cout << "CALLING INIT\n";
    MPI_Init(&argc, &argvtmp);

    std::cout << "AFTER\n";
    std::cout << "Argc: " << argc << "\n";
    for(int i = 0; i < argc; i++)
        std::cout << i << " : " << argv[i] << "\n";

    for(int i = 0; i < argc; i++)
      delete [] argvtmp[i];
    delete [] argvtmp;
}


void Finalize(void)
{
    MPI_Finalize();
}


void PrintInfo(void)
{
    std::cout << "I am a test mpi module\n";
}

void PrintMPIInfo(void)
{
    int rank = 0;
    int size = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::cout << "RANK: " << rank << " SIZE: " << size << "\n";
    for(int i = 0; i < 1000000000; i++);
}

}
