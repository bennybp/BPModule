include("${BPMODULE_CMAKE_PATH}/AddModule.cmake")
include("${BPMODULE_CMAKE_PATH}/Install.cmake")

ADD_MODULE(testmodule1 testmodule1.cpp "" "" "")
INSTALL_MODULE(testmodule1 "")
