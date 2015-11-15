cmake_minimum_required(VERSION 3.0)
include("${BPMODULE_PATH}/cmake/Common.cmake")
include("${BPMODULE_PATH}/cmake/Flags.cmake")
include("${BPMODULE_PATH}/cmake/AddModule.cmake")
include("${BPMODULE_PATH}/cmake/Install.cmake")

ADD_MODULE(testtensor testtensor.cpp "" "" "")
INSTALL_MODULE(testtensor "")
