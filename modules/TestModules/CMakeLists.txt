cmake_minimum_required(VERSION 3.0)
include("${BPMODULE_PATH}/cmake/Common.cmake")
include("${BPMODULE_PATH}/cmake/Flags.cmake")
include("${BPMODULE_PATH}/cmake/AddModule.cmake")
include("${BPMODULE_PATH}/cmake/Install.cmake")

# Create test libraries
add_library(dynlib SHARED dynlib.cpp)
add_library(staticlib STATIC staticlib.cpp)
set_target_properties(staticlib PROPERTIES COMPILE_FLAGS "-fPIC")


set(TESTMODULES_FILES creator.cpp
                      TestModule1.cpp
                      TestExtLib.cpp
   )

set (TESTMODULES_PYFILES creator.py
                         TestOptions.py
                         TestPyModule1.py
    )

ADD_SUPERMODULE(TestModules "${TESTMODULES_FILES}" "" "dynlib;staticlib" "")
INSTALL_SUPERMODULE(TestModules "${TESTMODULES_PYFILES}")

# install the dynamic library
install(TARGETS dynlib LIBRARY DESTINATION modules/TestModules)
