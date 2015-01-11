# Compiler definitions list need to be split
string(REPLACE ";" " " BPMODULE_CORE_CXX_FLAGS "${BPMODULE_CORE_CXX_FLAGS}")


##################################################
# Actual library
##################################################
add_library(parallel_base STATIC Parallel_Base.cpp )
set_target_properties(parallel_base PROPERTIES COMPILE_FLAGS "${BPMODULE_CORE_CXX_FLAGS}")
set_target_properties(parallel_base PROPERTIES INCLUDE_DIRECTORIES "${BPMODULE_CORE_CXX_INCLUDES}")

add_library(test_base STATIC Test_Base.cpp )
set_target_properties(test_base PROPERTIES COMPILE_FLAGS "${BPMODULE_CORE_CXX_FLAGS}")
set_target_properties(test_base PROPERTIES INCLUDE_DIRECTORIES "${BPMODULE_CORE_CXX_INCLUDES}")

# For now, install all header files
#file(GLOB HFILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
#install(FILES ${HFILES} DESTINATION include/panache)
#install(TARGETS panache ARCHIVE DESTINATION lib)
