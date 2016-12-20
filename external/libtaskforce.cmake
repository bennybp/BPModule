include(ExternalProject)

# Find if it already exists? (it's not required yet)
find_package(libtaskforce QUIET)

if(NOT libtaskforce_FOUND)
    message(STATUS "libtaskforce not found - we will build it for you")
   
    ExternalProject_Add(libtaskforce_external
        GIT_REPOSITORY https://github.com/ryanmrichard/LibTaskForce.git
        GIT_TAG f98fce1bf255595369483cea1b782402fa2f0fb5
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
                   -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                   -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                   -DMPI_CXX_COMPILER=${MPI_CXX_COMPILER}
                   -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
                   -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
                   -DCMAKE_POSITION_INDEPENDENT_CODE=True
        CMAKE_CACHE_ARGS -DCMAKE_PREFIX_PATH:LIST=${CMAKE_PREFIX_PATH}
        INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install DESTDIR=${CMAKE_BINARY_DIR}/stage
    )
else()
    message(STATUS "Found existing libtaskforce")
    message(STATUS "libtaskforce config: ${libtaskforce_CONFIG}")
    add_library(libtaskforce_external INTERFACE)
endif()
