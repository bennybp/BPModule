include(CTest)
SET(CTEST_BINARY_DIRECTORY "${CMAKE_INSTALL_DIR}")

# Macro for running a test via Pulsar's RunTest script
function(pulsar_runtest test_name test_path)
  set(extra_paths ${ARGN})
  add_test(NAME ${test_name}
      COMMAND ${PYTHON_EXECUTABLE} ${PULSAR_RUNTEST} ${test_path} ${extra_paths}
  )

endfunction()

# Macro for defining a python test
function(pulsar_py_test dir test_name)
  install(FILES ${test_name}.py DESTINATION ${dir})
  set(${test_name}_PY_PATH ${CMAKE_INSTALL_PREFIX}/${dir}/${test_name}.py)
  pulsar_runtest(${test_name}_PY ${${test_name}_PY_PATH} ${ARGN})
endfunction()

# Macro for defining a C++ test
function(pulsar_cxx_test dir test_name)
  add_library(${test_name} MODULE ${test_name}.cpp)
  target_include_directories(${test_name} PRIVATE ${PROJECT_SOURCE_DIR})
  target_link_libraries(${test_name} PRIVATE pulsar)
  install(TARGETS ${test_name} DESTINATION ${dir})
  set(${test_name}_CPP_PATH ${CMAKE_INSTALL_PREFIX}/${dir}/$<TARGET_FILE_NAME:${test_name}>)
  pulsar_runtest(${test_name}_CPP ${${test_name}_CPP_PATH} ${ARGN})
endfunction()

# Macro for defining both a Python and C++ test
function(pulsar_test dir test_name)
  pulsar_py_test(${dir} ${test_name} ${ARGN})
  pulsar_cxx_test(${dir} ${test_name} ${ARGN})
endfunction()
