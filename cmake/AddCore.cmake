include("cmake/Common.cmake")

# Adds a core library target
macro(ADD_CORELIB LIB_NAME
                  LIB_FILES
                  LIB_CXX_FLAGS
                  LIB_CXX_INCLUDES)

  message(STATUS "Adding core ${LIB_NAME}")
  add_library(${LIB_NAME} OBJECT ${LIB_FILES})

  # CXX Flags
  list(APPEND ${LIB_NAME}_CXX_FLAGS "${LIB_CXX_FLAGS}")
  list(APPEND ${LIB_NAME}_CXX_FLAGS "${OpenMP_CXX_FLAGS}")
  list(APPEND ${LIB_NAME}_CXX_FLAGS "-fPIC")
  list(APPEND ${LIB_NAME}_CXX_FLAGS "-std=c++11;-Wall;-pedantic")
  string(REPLACE ";" " " ${LIB_NAME}_CXX_FLAGS "${${LIB_NAME}_CXX_FLAGS}")
  set_target_properties(${LIB_NAME} PROPERTIES COMPILE_FLAGS "${${LIB_NAME}_CXX_FLAGS}")
  message(STATUS "${LIB_NAME} cxx compile flags: ${${LIB_NAME}_CXX_FLAGS}")

  # Includes
  list(APPEND ${LIB_NAME}_CXX_INCLUDES "${BPMODULE_PATH}")
  list(APPEND ${LIB_NAME}_CXX_INCLUDES "${LIB_CXX_INCLUDES}")
  list(APPEND ${LIB_NAME}_CXX_INCLUDES "${Boost_INCLUDE_DIRS}")
  list(APPEND ${LIB_NAME}_CXX_INCLUDES "${PYTHON_INCLUDE_DIRS}")
  set_target_properties(${LIB_NAME} PROPERTIES INCLUDE_DIRECTORIES "${${LIB_NAME}_CXX_INCLUDES}")
  message(STATUS "${LIB_NAME} cxx includes: ${${LIB_NAME}_CXX_INCLUDES}")

endmacro()
