 cmake_minimum_required(VERSION 2.8)
project(TensorLib CXX)
include(ExternalProject)

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
   ExternalProject_Add(${TENSOR_BACKEND}
      PREFIX "${TBE_SRC}"
      GIT_REPOSITORY https://github.com/ValeevGroup/tiledarray.git
      GIT_TAG 3450d49211521f43892ad5aabdfeaf345f247c20
      UPDATE_COMMAND ""
      CMAKE_COMMAND ${CMAKE_COMMAND}
      CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${TBE_SRC}
              -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
              -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
              -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
              -DBLAS_LIBRARIES=-mkl
      INSTALL_DIR "${TBE_SRC}"
   )
   set(TA_BUILD ${TBE_SRC}/src/TiledArray-build)
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


set(TensorHeaders TensorSetttings.hpp
                  TAImpl.hpp
                  Tensor.hpp
)

add_executable(TLTest Main.cc)

target_link_libraries(TLTest ${TBE_LIBRARY_FILES} ${INTEL_TBB} ${BLAS})
add_dependencies(TLTest ${TENSOR_BACKEND})
