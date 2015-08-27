macro(INSTALL_PYMODULE MODULE_NAME
                       ADD_PYTHON_FILES)

  list(APPEND ${MODULE_NAME}_PYFILES "__init__.py") 
  list(APPEND ${MODULE_NAME}_PYFILES "modinfo.py") 
  list(APPEND ${MODULE_NAME}_PYFILES "${ADD_PYTHON_FILES}") 
  install(FILES ${${MODULE_NAME}_PYFILES} DESTINATION modules/${MODULE_NAME}) 
endmacro()


macro(INSTALL_MODULE MODULE_NAME
                     ADD_PYTHON_FILES)

  INSTALL_PYMODULE(${MODULE_NAME} "${ADD_PYTHON_FILES}")

  install(TARGETS ${MODULE_NAME} LIBRARY DESTINATION modules/${MODULE_NAME}
                                 RUNTIME DESTINATION modules/${MODULE_NAME})
endmacro()
