 cmake_minimum_required(VERSION 2.8)
project(TensorLib CXX)
include(ExternalProject)

find_package(MPI REQUIRED)
message(STATUS ${MPI_CXX_LIBRARIES})
message(STATUS ${MPI_LINK_FLAGS})
set(INTEL_ROOT /theoryfs2/common/software/intel2015/composer_xe_2015.3.187)
set(INTEL_TBB ${INTEL_ROOT}/tbb/lib/intel64/gcc4.4/libtbb.so)
set(BLAS -mkl)

#The tensor backend, henceforth TBE
#Right now can be tiledarray (TA)
#or Cyclops Tensor Framework (CTF)
set(TENSOR_BACKEND TA)

#The path to the TBE's header files
set(TBE_HEADER_FILES "")

#The path to the TBE's libraries
set(TBE_LIBRARY_FILES "")

#We require C++11 because TA and CTF do
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(${TENSOR_BACKEND} STREQUAL TA)
   SET(TBE_SRC ${CMAKE_CURRENT_BINARY_DIR}/TiledArray)
   #ExternalProject_Add(madness
   #   PREFIX "${CMAKE_CURRENT_BINARY_DIR}/Madness"
   #   GIT_REPOSITORY https://github.com/m-a-d-n-e-s-s/madness
   #   GIT_TAG 2383898aa6701beb1205d86ac2cf424cf8611933
   #   UPDATE_COMMAND ""
   #   CMAKE_COMMAND ${CMAKE_COMMAND}
   #   CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}
   #           -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
   #           -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
   #           -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
   #           -DBLAS_LIBRARIES=-mkl
   #           -DENABLE_UNITTESTS=FALSE
   #   INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}
   #)
#-DMadness_ROOT_DIR=${CMAKE_CURRENT_BINARY_DIR}/Madness
      #DEPENDS Madness
   ExternalProject_Add(${TENSOR_BACKEND}
      PREFIX "${TBE_SRC}"
      GIT_REPOSITORY https://github.com/ValeevGroup/tiledarray.git
      GIT_TAG 07cd88bae7802f3ce1fcd4d8317151a873ecbffb
      UPDATE_COMMAND ""
      CMAKE_COMMAND ${CMAKE_COMMAND}
      CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${TBE_SRC}
                 -DCMAKE_PREFIX_PATH=${CMAKE_CURRENT_BINARY_DIR}
                 -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                 -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                 -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
                 -DBLAS_LIBRARIES=-mkl
      INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}
   )
   set(TA_BUILD ${TBE_SRC})
   set(TBE_HEADER_FILES ${TA_BUILD}/include
                       ${TA_BUILD}/include/eigen3
   )
set(TBE_LIBRARY_FILES ${TA_BUILD}/lib/libMADworld.so
                      ${TA_BUILD}/lib/libMADlinalg.so
                      ${TA_BUILD}/lib/libMADmisc.so
                      ${TA_BUILD}/lib/libMADtensor.so
   )
elseif(${TENSOR_BACKEND} STREQUAL CTF)
   SET(TBE_SRC ${CMAKE_CURRENT_BINARY_DIR}/CTF)
   ExternalProject_Add(${TENSOR_BACKEND}
      PREFIX "${TBE_SRC}"
      GIT_REPOSITORY https://github.com/solomonik/ctf
      GIT_TAG f67b1c8f028b7b3a2ea60a2751257dfe0225e9cc
      UPDATE_COMMAND ""
      CONFIGURE_COMMAND ${TBE_SRC}/src/CTF/configure
      BUILD_COMMAND make
      INSTALL_COMMAND ""
   )
endif()

include_directories(${TBE_HEADER_FILES})


set(TensorHeaders Shape.hpp
                  TensorSetttings.hpp
                  TAImpl.hpp
                  Tensor.hpp
)

add_executable(TLTest Main.cc)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${MPI_LINK_FLAGS}")
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include ${MPI_INCLUDE_PATH})
target_link_libraries(TLTest ${TBE_LIBRARY_FILES} 
   ${INTEL_TBB} ${BLAS} ${MPI_CXX_LIBRARIES})
add_dependencies(TLTest ${TENSOR_BACKEND})
