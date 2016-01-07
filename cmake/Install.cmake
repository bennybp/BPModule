macro(INSTALL_COREMODULE MODULE_NAME)

  install(TARGETS ${MODULE_NAME} LIBRARY DESTINATION modules/bpmodule/${MODULE_NAME}
                                 RUNTIME DESTINATION modules/bpmodule/${MODULE_NAME})

  install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/" DESTINATION modules/bpmodule/${MODULE_NAME} FILES_MATCHING PATTERN "*.py")

  # Headers for external modules to build against
  install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/" DESTINATION include/bpmodule/${MODULE_NAME} FILES_MATCHING PATTERN "*.hpp")
  install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/" DESTINATION include/bpmodule/${MODULE_NAME} FILES_MATCHING PATTERN "*.h")

endmacro()



macro(INSTALL_SUPERMODULE MODULE_NAME ADD_FILES)


  install(TARGETS ${MODULE_NAME} LIBRARY DESTINATION modules/${MODULE_NAME}
                                 RUNTIME DESTINATION modules/${MODULE_NAME})

  install(FILES "__init__.py"  DESTINATION modules/${MODULE_NAME})
  install(FILES "modinfo.py"   DESTINATION modules/${MODULE_NAME})
  install(FILES ${ADD_FILES}   DESTINATION modules/${MODULE_NAME})

  # Use this variable to get to your module's install path
  set(${MODULE_NAME}_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/modules/${MODULE_NAME})

  # Todo - set rpath here?

endmacro()
