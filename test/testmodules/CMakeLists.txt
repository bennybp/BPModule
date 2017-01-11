# Create test libraries for static/shared linking
add_library(testmodules_dynlib SHARED dynlib.cpp)
add_library(testmodules_staticlib STATIC staticlib.cpp)

set(TESTMODULES_FILES creator.cpp)

set (TESTMODULES_PYFILES creator.py TestPyModules.py)

# We only need this cpp file - all the rest is in the header
add_library(testmodules MODULE creator.cpp)
target_link_libraries(testmodules testmodules_dynlib
                                  testmodules_staticlib
                                  pulsar
                     )

# Make sure testmodules can find the dynlib library
set_property(TARGET testmodules
             PROPERTY INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/testmodules)

install(TARGETS testmodules LIBRARY DESTINATION testmodules
                            RUNTIME DESTINATION testmodules)

install(FILES "__init__.py" DESTINATION testmodules)
install(FILES "modinfo.py"  DESTINATION testmodules)
install(FILES creator.py TestPyModules.py DESTINATION testmodules)
                            

# install the dynamic library
install(TARGETS testmodules_dynlib LIBRARY DESTINATION testmodules
                                   RUNTIME DESTINATION testmodules)

