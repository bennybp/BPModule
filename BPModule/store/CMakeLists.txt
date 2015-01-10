##################################################
# Actual library
##################################################
add_library(bpmodule_store 
            STATIC
            ModuleStore.cpp
            )

add_library(bpmodule_storepy
            MODULE
            ModuleStore_python.cpp
           )

##############################################
# Panache library compiler flags, etc
##############################################
# Compiler definitions list need to be split
string(REPLACE ";" " " BPMODULE_STORE_CXX_FLAGS "${BPMODULE_STORE_CXX_FLAGS}")
set_target_properties(bpmodule_store PROPERTIES COMPILE_FLAGS "${BPMODULE_STORE_CXX_FLAGS}")
set_target_properties(bpmodule_store PROPERTIES INCLUDE_DIRECTORIES "${BPMODULE_STORE_CXX_INCLUDES}")

string(REPLACE ";" " " BPMODULE_STORE_CXX_FLAGS "${BPMODULE_STORE_CXX_FLAGS}")
set_target_properties(bpmodule_storepy PROPERTIES COMPILE_FLAGS "${BPMODULE_STORE_CXX_FLAGS}")
set_target_properties(bpmodule_storepy PROPERTIES INCLUDE_DIRECTORIES "${BPMODULE_STORE_CXX_INCLUDES}")
target_link_libraries(bpmodule_storepy bpmodule_store bpmodule_base)  # order is important!


# For now, install all header files
#file(GLOB HFILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
#install(FILES ${HFILES} DESTINATION include/panache)
#install(TARGETS panache ARCHIVE DESTINATION lib)
