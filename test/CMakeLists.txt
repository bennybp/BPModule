cmake_minimum_required(VERSION 3.0)
include(CTest)
enable_testing()

#Macro for defining a python test
function(pulsar_py_test test_name)
add_test(NAME ${test_name}
         COMMAND ${PYTHON_EXECUTABLE} ${test_name}.py
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
endfunction()

find_package(pulsar REQUIRED)
add_subdirectory(math)
configure_file("TestFxns.py.in" "TestFxns.py" @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/TestFxns.py
        DESTINATION ${CMAKE_INSTALL_PREFIX}/test)
install(FILES ${CMAKE_BINARY_DIR}/CTestTestfile.cmake 
        DESTINATION ${CMAKE_INSTALL_PREFIX}
)
install(DIRECTORY ${CMAKE_BINARY_DIR}/math 
        DESTINATION ${CMAKE_INSTALL_PREFIX}
)