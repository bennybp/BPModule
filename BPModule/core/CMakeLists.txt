##################################################
# Actual library
##################################################
add_library(bpmodule_core
            MODULE
            ModuleBase.cpp
            ModuleClass.cpp
            ModuleStore.cpp
            )

add_library(bpmodule_python
            MODULE
            Module_python.cpp
           )

add_library(bpmodule_pythonloader
            MODULE
            Python_Loader.cpp
           )


##############################################
# Compiler flags, etc
##############################################
# Compiler definitions list need to be split
string(REPLACE ";" " " BPMODULE_CORE_CXX_FLAGS "${BPMODULE_CORE_CXX_FLAGS}")

set_target_properties(bpmodule_core PROPERTIES COMPILE_FLAGS "${BPMODULE_CORE_CXX_FLAGS}")
set_target_properties(bpmodule_core PROPERTIES INCLUDE_DIRECTORIES "${BPMODULE_CORE_CXX_INCLUDES}")

set_target_properties(bpmodule_python PROPERTIES COMPILE_FLAGS "${BPMODULE_CORE_CXX_FLAGS}")
set_target_properties(bpmodule_python PROPERTIES INCLUDE_DIRECTORIES "${BPMODULE_CORE_CXX_INCLUDES}")

set_target_properties(bpmodule_pythonloader PROPERTIES COMPILE_FLAGS "${BPMODULE_CORE_CXX_FLAGS}")
set_target_properties(bpmodule_pythonloader PROPERTIES INCLUDE_DIRECTORIES "${BPMODULE_CORE_CXX_INCLUDES}")

# For now, install all header files
#file(GLOB HFILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
#install(FILES ${HFILES} DESTINATION include/panache)
#install(TARGETS panache ARCHIVE DESTINATION lib)
