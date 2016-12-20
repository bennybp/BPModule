include(ExternalProject)

# Find if it already exists? (it's not required yet)
find_package(pybind11 QUIET) 

# If not found, build it
if(NOT pybind11_FOUND)
    message(STATUS "Pybind11 not found - we will build it for you")

    ExternalProject_Add(pybind11_external
        GIT_REPOSITORY https://github.com/pybind/pybind11.git
        GIT_TAG c79e435e00fd4fb49817a87c3c2b2647482c077b
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
                   -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                   -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                   -DPYBIND11_TEST=False
                   -DPYBIND11_INSTALL=True
        INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install DESTDIR=${STAGE_DIR}
    )

else()
    message(STATUS "Found existing pybind11")
    message(STATUS "pybind11 config: ${pybind11_CONFIG}")
    add_library(pybind11_external INTERFACE)
endif()
