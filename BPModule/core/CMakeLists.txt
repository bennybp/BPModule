##################################################
# Actual library
##################################################
add_library(bpmodule_core
            STATIC
            ModuleBase.cpp
            ModuleClass.cpp
            ModuleStore.cpp
            )

add_library(bpmodule_store
            MODULE
            ModuleStore_global.cpp
           )

##############################################
# Compiler flags, etc
##############################################
# Compiler definitions list need to be split
string(REPLACE ";" " " BPMODULE_CORE_CXX_FLAGS "${BPMODULE_CORE_CXX_FLAGS}")

set_target_properties(bpmodule_core PROPERTIES COMPILE_FLAGS "${BPMODULE_CORE_CXX_FLAGS}")
set_target_properties(bpmodule_core PROPERTIES INCLUDE_DIRECTORIES "${BPMODULE_CORE_CXX_INCLUDES}")

set_target_properties(bpmodule_store PROPERTIES COMPILE_FLAGS "${BPMODULE_CORE_CXX_FLAGS}")
set_target_properties(bpmodule_store PROPERTIES INCLUDE_DIRECTORIES "${BPMODULE_CORE_CXX_INCLUDES}")
target_link_libraries(bpmodule_store bpmodule_core)

# For now, install all header files
#file(GLOB HFILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
#install(FILES ${HFILES} DESTINATION include/panache)
#install(TARGETS panache ARCHIVE DESTINATION lib)
