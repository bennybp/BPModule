macro(ADD_MODULE MODULE_NAME
                 MODULE_FILES
                 ADD_CXX_FLAGS
                 ADD_CXX_LINK_FLAGS
                 ADD_CXX_INCLUDES)

  message(STATUS "Adding module ${MODULE_NAME}")
  add_library(${MODULE_NAME} MODULE ${MODULE_FILES} )

  # CXX Flags
  list(APPEND ${MODULE_NAME}_CXX_FLAGS "${BPMODULE_CXX_FLAGS}")
  list(APPEND ${MODULE_NAME}_CXX_FLAGS "${ADD_CXX_FLAGS}")
  string(REPLACE ";" " " ${MODULE_NAME}_CXX_FLAGS "${${MODULE_NAME}_CXX_FLAGS}")
  message(STATUS "${MODULE_NAME} cxx compile flags: ${${MODULE_NAME}_CXX_FLAGS}")
  set_target_properties(${MODULE_NAME} PROPERTIES COMPILE_FLAGS "${${MODULE_NAME}_CXX_FLAGS}")

  # Includes
  list(APPEND ${MODULE_NAME}_CXX_INCLUDES "${BPMODULE_CXX_INCLUDES}")
  list(APPEND ${MODULE_NAME}_CXX_INCLUDES "${ADD_CXX_INCLUDES}")
  message(STATUS "${MODULE_NAME} cxx includes: ${${MODULE_NAME}_CXX_INCLUDES}")
  set_target_properties(${MODULE_NAME} PROPERTIES INCLUDE_DIRECTORIES "${${MODULE_NAME}_CXX_INCLUDES}")

  # Linker flags
  list(APPEND ${MODULE_NAME}_CXX_LINK_FLAGS "${BPMODULE_CXX_LINK_FLAGS}")
  list(APPEND ${MODULE_NAME}_CXX_LINK_FLAGS "${ADD_CXX_LINK_FLAGS}")
  message(STATUS "${MODULE_NAME} cxx link flags: ${${MODULE_NAME}_CXX_LINK_FLAGS}")
  target_link_libraries(${MODULE_NAME} "${${MODULE_NAME}_CXX_LINK_FLAGS}") 
endmacro()

add_subdirectory(core)
add_subdirectory(modules)
