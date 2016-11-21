cmake_minimum_required(VERSION 3.0)
include(CTest)

SET(CTEST_BINARY_DIRECTORY "${CMAKE_INSTALL_DIR}")

#Should find the Pulsar in the staging area
#This sets PULSAR_RUNTEST variable
find_package(pulsar REQUIRED)

# The path to the tests
set(PULSAR_TEST_PATH ${CMAKE_INSTALL_PREFIX}/test)

#Macro for running a test via Pulsar's RunTest script
function(pulsar_runtest test_name test_path)
  add_test(NAME ${test_name}
           COMMAND ${PYTHON_EXECUTABLE} ${PULSAR_RUNTEST} ${test_path}
           WORKING_DIRECTORY ${PULSAR_TEST_PATH}
          )

endfunction()

#Macro for defining a python test
function(pulsar_py_test test_name)
  install(FILES ${test_name}.py DESTINATION test)
  set(${test_name}_PY_PATH ${PULSAR_TEST_PATH}/${test_name}.py)
  pulsar_runtest(${test_name}_PY ${PULSAR_TEST_PATH}/${test_name}.py)
endfunction()


#Macro for defining a C++ test
function(pulsar_cxx_test test_name)
  add_library(${test_name} MODULE ${test_name}.cpp)
  target_include_directories(${test_name} PRIVATE ${PROJECT_SOURCE_DIR})
  target_link_libraries(${test_name} PRIVATE pulsar)
  install(TARGETS ${test_name} DESTINATION test)
  pulsar_runtest(${test_name}_CPP $<TARGET_FILE:${test_name}>) 
endfunction()

#Macro for defining both a Python and C++ test
function(pulsar_test test_name)
   pulsar_py_test(${test_name})
   pulsar_cxx_test(${test_name})
endfunction()

#This file will allow us to run ctest in the top-level build dir
#Basically it just defers to the actual top-level CTestTestfile.cmake in the
#build directory for this project
file(WRITE ${CMAKE_INSTALL_PREFIX}/CTestTestfile.cmake "subdirs(test)")


########################
# Core pulsar tests
########################
foreach(dir math system modules)
  add_subdirectory(${dir})
endforeach()

