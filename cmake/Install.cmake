macro(INSTALL_COREPYMODULE MODULE_NAME
                       ADD_PYTHON_FILES)

  list(APPEND ${MODULE_NAME}_PYFILES "__init__.py") 
  list(APPEND ${MODULE_NAME}_PYFILES "modinfo.py") 
  list(APPEND ${MODULE_NAME}_PYFILES "${ADD_PYTHON_FILES}") 
  install(FILES ${${MODULE_NAME}_PYFILES} DESTINATION modules/bpmodule/${MODULE_NAME}) 
endmacro()


macro(INSTALL_COREMODULE MODULE_NAME
                     HEADER_FILES
                     ADD_PYTHON_FILES)

  INSTALL_COREPYMODULE(${MODULE_NAME} "${ADD_PYTHON_FILES}")

  install(TARGETS ${MODULE_NAME} LIBRARY DESTINATION modules/bpmodule/${MODULE_NAME}
                                 RUNTIME DESTINATION modules/bpmodule/${MODULE_NAME})

  install(FILES ${HEADER_FILES} DESTINATION include/bpmodule/${MODULE_NAME})
endmacro()



macro(INSTALL_PYSUPERMODULE MODULE_NAME
                       ADD_PYTHON_FILES)

  list(APPEND ${MODULE_NAME}_PYFILES "__init__.py") 
  list(APPEND ${MODULE_NAME}_PYFILES "modinfo.py") 
  list(APPEND ${MODULE_NAME}_PYFILES "${ADD_PYTHON_FILES}") 
  install(FILES ${${MODULE_NAME}_PYFILES} DESTINATION modules/${MODULE_NAME}) 
endmacro()


macro(INSTALL_SUPERMODULE MODULE_NAME
                          ADD_PYTHON_FILES)

  INSTALL_PYSUPERMODULE(${MODULE_NAME} "${ADD_PYTHON_FILES}")

  install(TARGETS ${MODULE_NAME} LIBRARY DESTINATION modules/${MODULE_NAME}
                                 RUNTIME DESTINATION modules/${MODULE_NAME})

  # Use this variable to get to your module's install path
  set(${MODULE_NAME}_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/modules/${MODULE_NAME})

  # Todo - set rpath here?

endmacro()
