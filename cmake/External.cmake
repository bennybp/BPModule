#####################################
# Projects in the external directory
#####################################

##################
# Pybind11
##################
add_library(pybind INTERFACE)
target_include_directories(pybind SYSTEM INTERFACE
       $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/pybind11/include>
       $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/external>
)
install(TARGETS pybind EXPORT bpmodule)
install(DIRECTORY external/pybind11/include/pybind11 DESTINATION ${CMAKE_INSTALL_PREFIX}/external USE_SOURCE_PERMISSIONS)

