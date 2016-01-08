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
add_library(bpmodule_headers INTERFACE)
target_include_directories(bpmodule_headers INTERFACE
                           $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
)


# Add to the bpmodule export
install(TARGETS bpmodule_headers EXPORT bpmodule)

# Install all python files (for all core modules)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" DESTINATION modules FILES_MATCHING PATTERN "*.py")

# Install all headers (for all core modules)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" DESTINATION include FILES_MATCHING PATTERN "*.h")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" DESTINATION include FILES_MATCHING PATTERN "*.hpp")
