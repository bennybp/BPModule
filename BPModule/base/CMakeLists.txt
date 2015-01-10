##################################################
# Actual library
##################################################
add_library(bpmodule_base
            STATIC
            ModuleBase.cpp
            ModuleClass.cpp
            )

##############################################
# Compiler flags, etc
##############################################
# Compiler definitions list need to be split
string(REPLACE ";" " " BPMODULE_BASE_CXX_FLAGS "${BPMODULE_BASE_CXX_FLAGS}")
set_target_properties(bpmodule_base PROPERTIES COMPILE_FLAGS "${BPMODULE_BASE_CXX_FLAGS}")
set_target_properties(bpmodule_base PROPERTIES INCLUDE_DIRECTORIES "${BPMODULE_BASE_CXX_INCLUDES}")

# For now, install all header files
#file(GLOB HFILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
#install(FILES ${HFILES} DESTINATION include/panache)
#install(TARGETS panache ARCHIVE DESTINATION lib)
