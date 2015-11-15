cmake_minimum_required(VERSION 3.0)
include("${BPMODULE_PATH}/cmake/Common.cmake")
include("${BPMODULE_PATH}/cmake/Flags.cmake")
include("${BPMODULE_PATH}/cmake/AddModule.cmake")
include("${BPMODULE_PATH}/cmake/Install.cmake")

# Create test libraries
add_library(staticlib STATIC staticlib.cpp)
set_target_properties(staticlib PROPERTIES COMPILE_FLAGS "-fPIC")

add_library(dynlib SHARED dynlib.cpp)

ADD_MODULE(testextlib testextlib.cpp "" "dynlib;staticlib" "")
INSTALL_MODULE(testextlib "")

install(TARGETS dynlib LIBRARY DESTINATION modules/testextlib)

