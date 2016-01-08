# This is treated a little differently since it
# is written in strict C

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "INTEL")
  list(APPEND BPMODULE_MEMWATCH_C_FLAGS "-fPIC")
  list(APPEND BPMODULE_MEMWATCH_C_FLAGS "-std=c99")
  list(APPEND BPMODULE_MEMWATCH_C_FLAGS "-w3")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
  list(APPEND BPMODULE_MEMWATCH_C_FLAGS "-fPIC")
  list(APPEND BPMODULE_MEMWATCH_C_FLAGS "-std=c99")
  list(APPEND BPMODULE_MEMWATCH_C_FLAGS "-Wall;-Wextra;-pedantic")
  list(APPEND BPMODULE_MEMWATCH_C_FLAGS "-Wshadow")
endif()

add_library(memwatch SHARED memwatch.c)
target_link_libraries(memwatch dl)
install(TARGETS memwatch 
        EXPORT bpmodule
        LIBRARY DESTINATION memwatch)
#install(FILES memwatch.h DESTINATION memwatch)
