# Find boost and python
find_package(Boost COMPONENTS python3 REQUIRED)

set(Python_ADDITIONAL_VERSIONS "3.3;3.4")
find_package(PythonLibs REQUIRED)



################
# Define macros
################
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



macro(INSTALL_MODULE MODULE_NAME
                     ADD_PYTHON_FILES)

  list(APPEND ${MODULE_NAME}_PYFILES "python_module/__init__.py") 
  list(APPEND ${MODULE_NAME}_PYFILES "python_module/modinfo.py") 
  list(APPEND ${MODULE_NAME}_PYFILES "${ADD_PYTHON_FILES}") 
  install(TARGETS ${MODULE_NAME} LIBRARY DESTINATION modules/${MODULE_NAME}
                                 RUNTIME DESTINATION modules/${MODULE_NAME})
  install(FILES ${${MODULE_NAME}_PYFILES} DESTINATION modules/${MODULE_NAME}) 
endmacro()
    




macro(ADD_CORELIB LIB_NAME
                  LIB_FILES
                  ADD_CXX_FLAGS
                  ADD_CXX_LINK_FLAGS
                  ADD_CXX_INCLUDES)

  message(STATUS "Adding core ${LIB_NAME}")
  add_library(${LIB_NAME} OBJECT ${LIB_FILES})

  # CXX Flags
  list(APPEND ${LIB_NAME}_CXX_FLAGS "${BPMODULE_CXX_FLAGS}")
  list(APPEND ${LIB_NAME}_CXX_FLAGS "-fPIC")
  list(APPEND ${LIB_NAME}_CXX_FLAGS "${ADD_CXX_FLAGS}")
  string(REPLACE ";" " " ${LIB_NAME}_CXX_FLAGS "${${LIB_NAME}_CXX_FLAGS}")
  message(STATUS "${LIB_NAME} cxx compile flags: ${${LIB_NAME}_CXX_FLAGS}")
  set_target_properties(${LIB_NAME} PROPERTIES COMPILE_FLAGS "${${LIB_NAME}_CXX_FLAGS}")

  # Includes
  list(APPEND ${LIB_NAME}_CXX_INCLUDES "${BPMODULE_CXX_INCLUDES}")
  list(APPEND ${LIB_NAME}_CXX_INCLUDES "${Boost_INCLUDE_DIRS}")
  list(APPEND ${LIB_NAME}_CXX_INCLUDES "${PYTHON_INCLUDE_DIRS}")
  list(APPEND ${LIB_NAME}_CXX_INCLUDES "${ADD_CXX_INCLUDES}")
  message(STATUS "${LIB_NAME} cxx includes: ${${LIB_NAME}_CXX_INCLUDES}")
  set_target_properties(${LIB_NAME} PROPERTIES INCLUDE_DIRECTORIES "${${LIB_NAME}_CXX_INCLUDES}")

  # Linker flags
  list(APPEND ${LIB_NAME}_CXX_LINK_FLAGS "${BPMODULE_CXX_LINK_FLAGS}")
  list(APPEND ${LIB_NAME}_CXX_LINK_FLAGS "${Boost_LIBRARIES}")
  list(APPEND ${LIB_NAME}_CXX_LINK_FLAGS "${PYTHON_LIBRARIES_LIBRARIES}")
  list(APPEND ${LIB_NAME}_CXX_LINK_FLAGS "${ADD_CXX_LINK_FLAGS}")
  message(STATUS "${LIB_NAME} cxx link flags: ${${LIB_NAME}_CXX_LINK_FLAGS}")
endmacro()


add_subdirectory(core)
add_subdirectory(modulebase)
add_subdirectory(export_core)

