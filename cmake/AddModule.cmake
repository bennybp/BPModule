# Creates a target for a module
macro(ADD_MODULE MODULE_NAME
                 MODULE_FILES
                 MODULE_CXX_FLAGS
                 MODULE_CXX_LINK_FLAGS
                 MODULE_CXX_INCLUDES)

  if(NOT "${MODULE_FILES}" STREQUAL "")
      message(STATUS "Adding module ${MODULE_NAME}")
      add_library(${MODULE_NAME} MODULE ${MODULE_FILES} )

      # CXX Flags
      list(APPEND ${MODULE_NAME}_CXX_FLAGS "${MODULE_CXX_FLAGS}")
      list(APPEND ${MODULE_NAME}_CXX_FLAGS "${OpenMP_CXX_FLAGS}")
      list(APPEND ${MODULE_NAME}_CXX_FLAGS "-fPIC")
      list(APPEND ${MODULE_NAME}_CXX_FLAGS "-std=c++11;-Wall;-pedantic")
      string(REPLACE ";" " " ${MODULE_NAME}_CXX_FLAGS "${${MODULE_NAME}_CXX_FLAGS}")
      set_target_properties(${MODULE_NAME} PROPERTIES COMPILE_FLAGS "${${MODULE_NAME}_CXX_FLAGS}")
      message(STATUS "${MODULE_NAME} cxx compile flags: ${${MODULE_NAME}_CXX_FLAGS}")

      # Includes
      list(APPEND ${MODULE_NAME}_CXX_INCLUDES "${BPMODULE_PATH}")          # When building the core, in-source tree
      list(APPEND ${MODULE_NAME}_CXX_INCLUDES "${BPMODULE_PATH}/include")  # Out-of-source module build
      list(APPEND ${MODULE_NAME}_CXX_INCLUDES "${MODULE_CXX_INCLUDES}")
      set_target_properties(${MODULE_NAME} PROPERTIES INCLUDE_DIRECTORIES "${${MODULE_NAME}_CXX_INCLUDES}")
      message(STATUS "${MODULE_NAME} cxx includes: ${${MODULE_NAME}_CXX_INCLUDES}")

      # Linker flags
      list(APPEND ${MODULE_NAME}_CXX_LINK_FLAGS "${MODULE_CXX_LINK_FLAGS}")
      list(APPEND ${MODULE_NAME}_CXX_LINK_FLAGS "${OpenMP_CXX_FLAGS}")
      target_link_libraries(${MODULE_NAME} "${${MODULE_NAME}_CXX_LINK_FLAGS}")
      message(STATUS "${MODULE_NAME} cxx link flags: ${${MODULE_NAME}_CXX_LINK_FLAGS}")

  endif()

endmacro()
