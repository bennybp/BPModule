##################################################
# Actual library
##################################################
add_library(testmodule 
            MODULE
            testmodule.cpp
            )

add_library(testmodule2
            MODULE
            testmodule2.cpp
            )

##############################################
# Panache library compiler flags, etc
##############################################
# Compiler definitions list need to be split
string(REPLACE ";" " " BPMODULE_CXX_FLAGS "${BPMODULE_CXX_FLAGS}")
set_target_properties(testmodule PROPERTIES COMPILE_FLAGS "${BPMODULE_CXX_FLAGS}")
set_target_properties(testmodule PROPERTIES INCLUDE_DIRECTORIES "${BPMODULE_CXX_INCLUDES}")
target_link_libraries(testmodule bpmodule_base)

set_target_properties(testmodule2 PROPERTIES COMPILE_FLAGS "${BPMODULE_CXX_FLAGS}")
set_target_properties(testmodule2 PROPERTIES INCLUDE_DIRECTORIES "${BPMODULE_CXX_INCLUDES}")
target_link_libraries(testmodule2 bpmodule_base)


# For now, install all header files
#file(GLOB HFILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
#install(FILES ${HFILES} DESTINATION include/panache)
#install(TARGETS panache ARCHIVE DESTINATION lib)
