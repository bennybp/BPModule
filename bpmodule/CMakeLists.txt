cmake_minimum_required(VERSION 3.0)


#################################
# Load macros
#################################
include("cmake/Flags.cmake")
include("cmake/Python.cmake")
include("cmake/Common.cmake")
include("cmake/AddModule.cmake")
include("cmake/Install.cmake")


add_subdirectory(python)
add_subdirectory(output)
add_subdirectory(util)
add_subdirectory(exception)
add_subdirectory(parallel)
add_subdirectory(math)
add_subdirectory(tensor)
add_subdirectory(datastore)
add_subdirectory(modulebase)
add_subdirectory(modulelocator)
add_subdirectory(molecule)
add_subdirectory(basisset)
add_subdirectory(testing)


######################################
# Make a header-only interface library
######################################
add_library(bpmodule_core_interface INTERFACE)
target_include_directories(bpmodule_core_interface INTERFACE
                           $<INSTALL_INTERFACE:include>
)


# Add to the bpmodule export
install(TARGETS bpmodule_core_interface EXPORT bpmodule)

# Install all python files (for all core modules)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" DESTINATION modules FILES_MATCHING PATTERN "*.py")

# Install all headers (for all core modules)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" DESTINATION include FILES_MATCHING PATTERN "*.h")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" DESTINATION include FILES_MATCHING PATTERN "*.hpp")
