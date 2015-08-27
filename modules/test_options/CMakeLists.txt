include("${BPMODULE_CMAKE_PATH}/AddModule.cmake")
include("${BPMODULE_CMAKE_PATH}/Install.cmake")

#ADD_MODULE(testpymodule1 "" "" "" "")
INSTALL_PYMODULE(test_options "testmod.py")
