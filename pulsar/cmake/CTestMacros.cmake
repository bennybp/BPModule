include(CTest)
SET(CTEST_BINARY_DIRECTORY "${CMAKE_INSTALL_DIR}")

# Macro for running a test via Pulsar's RunTest script
function(pulsar_runtest test_name test_path)
  set(extra_paths ${ARGN})
  add_test(NAME ${test_name}
      COMMAND ${PYTHON_EXECUTABLE} ${PULSAR_RUNTEST} ${test_path} ${extra_paths}
  )
endfunction()

# Macro for running a full input (ie, an example)
function(pulsar_runexample example_name example_path)
  set(extra_paths ${ARGN})
  add_test(NAME Example_${example_name}
      COMMAND ${PYTHON_EXECUTABLE} ${PULSAR_RUNEXAMPLE} ${example_path} ${extra_paths})
endfunction()

#Macro for building a library that will be used for testing
function(testing_library dir lib_name)
    add_library(${lib_name} MODULE ${lib_name}.cpp)
    target_include_directories(${lib_name} PRIVATE ${PROJECT_SOURCE_DIR})
    target_link_libraries(${lib_name} PRIVATE pulsar)
    install(TARGETS ${lib_name} DESTINATION ${dir})
endfunction()

#Macro for defining a python test
function(pulsar_py_test dir test_name)
  install(FILES ${test_name}.py DESTINATION ${dir})
  set(${test_name}_PY_PATH ${CMAKE_INSTALL_PREFIX}/${dir}/${test_name}.py)
  pulsar_runtest(${test_name}_PY ${${test_name}_PY_PATH} ${ARGN})
endfunction()

# Macro for defining a C++ test
function(pulsar_cxx_test dir test_name)
  testing_library(${dir} ${test_name})
  pulsar_runtest(${test_name}_CPP $<TARGET_FILE:${test_name}>)
endfunction()

# Macro for defining both a Python and C++ test
function(pulsar_test dir test_name)
  pulsar_py_test(${dir} ${test_name} ${ARGN})
  pulsar_cxx_test(${dir} ${test_name} ${ARGN})
endfunction()

# Macro for defining an example being run as a test
function(pulsar_example dir example_name)
  # This path is relative to the lib/pulsar/examples directory
  set(${example_name}_EX_PATH ${dir}/${example_name}.py)
  pulsar_runexample(${example_name} ${${example_name}_EX_PATH} ${ARGN})
endfunction()
