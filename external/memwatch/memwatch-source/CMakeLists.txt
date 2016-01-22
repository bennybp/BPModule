cmake_minimum_required(VERSION 3.0)

# This is treated a little differently since it
# is written in strict C

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "INTEL")
  list(APPEND MEMWATCH_C_FLAGS "-fPIC")
  list(APPEND MEMWATCH_C_FLAGS "-std=c99")
  list(APPEND MEMWATCH_C_FLAGS "-w3")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
  list(APPEND MEMWATCH_C_FLAGS "-fPIC")
  list(APPEND MEMWATCH_C_FLAGS "-std=c99")
  list(APPEND MEMWATCH_C_FLAGS "-Wall;-Wextra;-pedantic")
  list(APPEND MEMWATCH_C_FLAGS "-Wshadow")
endif()

add_library(memwatch SHARED memwatch.c)
target_link_libraries(memwatch PUBLIC dl)

install(TARGETS memwatch LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(FILES memwatch.h DESTINATION include/memwatch)
