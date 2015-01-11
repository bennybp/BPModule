##################################################
# Actual library
##################################################
add_library(parallel_serial 
            MODULE
            Parallel_Serial.cpp
            )


##############################################
# Panache library compiler flags, etc
##############################################
# Compiler definitions list need to be split
string(REPLACE ";" " " BPMODULE_CXX_FLAGS "${BPMODULE_CXX_FLAGS}")
set_target_properties(parallel_serial PROPERTIES COMPILE_FLAGS "${BPMODULE_CXX_FLAGS}")
set_target_properties(parallel_serial PROPERTIES INCLUDE_DIRECTORIES "${BPMODULE_CXX_INCLUDES}")
target_link_libraries(parallel_serial parallel_base bpmodule_core)

find_package(OpenMP)
if(OPENMP_FOUND)
  add_library(parallel_openmp
              MODULE
              Parallel_OpenMP.cpp
              )
  set_target_properties(parallel_openmp PROPERTIES COMPILE_FLAGS "${BPMODULE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set_target_properties(parallel_openmp PROPERTIES INCLUDE_DIRECTORIES "${BPMODULE_CXX_INCLUDES}")
  target_link_libraries(parallel_openmp parallel_base bpmodule_core ${OpenMP_CXX_FLAGS})
endif()

# For now, install all header files
#file(GLOB HFILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
#install(FILES ${HFILES} DESTINATION include/panache)
#install(TARGETS panache ARCHIVE DESTINATION lib)
