macro(INSTALL_COREMODULE MODULE_NAME)

  install(TARGETS ${MODULE_NAME} 
          LIBRARY DESTINATION modules/bpmodule/${MODULE_NAME}
          RUNTIME DESTINATION modules/bpmodule/${MODULE_NAME})

  # Python files are installed from bpmodule/CMakeLists.txt

endmacro()



macro(INSTALL_SUPERMODULE MODULE_NAME ADD_FILES)

  install(TARGETS ${MODULE_NAME} LIBRARY DESTINATION modules/${MODULE_NAME}
                                 RUNTIME DESTINATION modules/${MODULE_NAME})

  install(FILES "__init__.py"  DESTINATION modules/${MODULE_NAME})
  install(FILES "modinfo.py"   DESTINATION modules/${MODULE_NAME})
  install(FILES ${ADD_FILES}   DESTINATION modules/${MODULE_NAME})

  # Use this variable to get to your module's install path
  set(${MODULE_NAME}_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/modules/${MODULE_NAME})

endmacro()
