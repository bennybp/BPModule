# Creates a target for a module
macro(ADD_SUPERMODULE MODULE_NAME
                      MODULE_FILES
                      MODULE_CXX_FLAGS
                      MODULE_CXX_LINK_FLAGS
                      MODULE_CXX_INCLUDES)

  if(NOT "${MODULE_FILES}" STREQUAL "")
      message(STATUS "Adding module ${MODULE_NAME}")
      add_library(${MODULE_NAME} MODULE ${MODULE_FILES} )


      #################
      # CXX Flags
      #################
      list(APPEND ${MODULE_NAME}_CXX_FLAGS ${MODULE_CXX_FLAGS})

      # OpenMP
      list(APPEND ${MODULE_NAME}_CXX_FLAGS ${OpenMP_CXX_FLAGS})

      # MPI
      list(APPEND ${MODULE_NAME}_CXX_FLAGS ${MPI_CXX_COMPILE_FLAGS})

      # Default flags for stuff
      list(APPEND ${MODULE_NAME}_CXX_FLAGS ${BPMODULE_CXX_STRICT_FLAGS})
      target_compile_options(${MODULE_NAME} PRIVATE ${${MODULE_NAME}_CXX_FLAGS})
      #message(STATUS "${MODULE_NAME} cxx compile flags: ${${MODULE_NAME}_CXX_FLAGS}")



      #################
      # Includes
      #################
      list(APPEND ${MODULE_NAME}_CXX_INCLUDES ${CMAKE_SOURCE_DIR}/../) # TODO - HACK
      list(APPEND ${MODULE_NAME}_CXX_INCLUDES ${Boost_INCLUDE_DIRS})
      list(APPEND ${MODULE_NAME}_CXX_INCLUDES ${PYTHON_INCLUDE_DIRS})

      list(APPEND ${MODULE_NAME}_CXX_INCLUDES ${MPI_CXX_INCLUDE_PATH})


      list(APPEND ${MODULE_NAME}_CXX_INCLUDES ${MODULE_CXX_INCLUDES})
      target_include_directories(${MODULE_NAME} PRIVATE ${${MODULE_NAME}_CXX_INCLUDES})
      #message(STATUS "${MODULE_NAME} cxx includes: ${${MODULE_NAME}_CXX_INCLUDES}")



      #################
      # Linker flags
      #################
      list(APPEND ${MODULE_NAME}_CXX_LINK_FLAGS ${MODULE_CXX_LINK_FLAGS})
      list(APPEND ${MODULE_NAME}_CXX_LINK_FLAGS ${OpenMP_CXX_FLAGS})
      list(APPEND ${MODULE_NAME}_CXX_LINK_FLAGS ${Boost_LIBRARIES})
      list(APPEND ${MODULE_NAME}_CXX_LINK_FLAGS ${PYTHON_LIBRARIES})
      list(APPEND ${MODULE_NAME}_CXX_LINK_FLAGS ${BLAS_LIBRARIES})
      list(APPEND ${MODULE_NAME}_CXX_LINK_FLAGS ${LAPACK_LIBRARIES})

      list(APPEND ${MODULE_NAME}_CXX_LINK_FLAGS ${MPI_CXX_LINK_FLAGS})
      list(APPEND ${MODULE_NAME}_CXX_LINK_FLAGS ${MPI_CXX_LIBRARIES})

   
      # Libraries passed in
      target_link_libraries(${MODULE_NAME} ${${MODULE_NAME}_CXX_LINK_FLAGS})

      # Link against pybind interface library (will also modify include paths) 
      target_link_libraries(${MODULE_NAME} External::pybind11_interface)
      target_link_libraries(${MODULE_NAME} External::libelemental_interface)

      #message(STATUS "${MODULE_NAME} cxx link flags: ${${MODULE_NAME}_CXX_LINK_FLAGS}")

  endif()

endmacro()
