cmake_minimum_required(VERSION 3.0)
include(CTest)

SET(CTEST_BINARY_DIRECTORY "${CMAKE_INSTALL_DIR}")

#Should find the Pulsar in the staging area
find_package(pulsar REQUIRED)

set(PULSAR_RUNTEST_PATH ${PULSAR_INSTALL_ROOT}/modules/pulsar/testing)
set(PULSAR_RUNTEST ${PULSAR_RUNTEST_PATH}/RunTest.py)

#Macro for defining a python test
function(pulsar_py_test test_name)
  set(${test_name}_CPP_PATH ${CMAKE_INSTALL_PREFIX}/test/${test_name})
  add_test(NAME ${test_name}_PY
           COMMAND ${PYTHON_EXECUTABLE} ${PULSAR_RUNTEST} ${${test_name}_CPP_PATH}
           WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/test
          )

  install(FILES ${test_name}.py
          DESTINATION test
         )
endfunction()


#Macro for defining a C++ test
function(pulsar_cxx_test test_name)
  set(${test_name}_CPP_PATH ${CMAKE_INSTALL_PREFIX}/test/${test_name})
  add_library(${test_name} MODULE ${test_name}.cpp)
  target_include_directories(${test_name} PRIVATE ${PROJECT_SOURCE_DIR} pulsar)
  target_link_libraries(${test_name} PRIVATE pulsar)
  add_test(NAME ${test_name}_CPP
           COMMAND ${PYTHON_EXECUTABLE} ${PULSAR_RUNTEST} ${${test_name}_CPP_PATH}
           WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/test
          )

  install(TARGETS ${test_name}
          DESTINATION test
  )
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

# This file is imported by python tests
configure_file("TestFxns.py.in" "TestFxns.py" @ONLY)

########################
# Core pulsar tests
########################
foreach(dir math system)
  add_subdirectory(${dir})
endforeach()

install(FILES ${CMAKE_BINARY_DIR}/TestFxns.py
        DESTINATION ${CMAKE_INSTALL_PREFIX}/test)
