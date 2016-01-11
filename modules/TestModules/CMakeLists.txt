cmake_minimum_required(VERSION 3.0)
include("${BPMODULE_PATH}/cmake/bpmodule.cmake")

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





add_library(TestModules MODULE ${TESTMODULES_FILES})

target_link_libraries(TestModules dynlib staticlib bpmodule)

install(TARGETS TestModules LIBRARY DESTINATION modules/TestModules
                            RUNTIME DESTINATION modules/TestModules)

install(FILES "__init__.py" DESTINATION modules/TestModules)
install(FILES "modinfo.py"  DESTINATION modules/TestModules)
install(FILES ${TESTMODULES_PYFILES} DESTINATION modules/TestModules)
                            
set_target_properties(TestModules PROPERTIES INSTALL_RPATH modules/TestModules)

# install the dynamic library
install(TARGETS dynlib LIBRARY DESTINATION modules/TestModules)
