##################################################
# Actual library
##################################################

find_package(Boost COMPONENTS python3 REQUIRED)

set(Python_ADDITIONAL_VERSIONS "3.3;3.4")
find_package(PythonLibs REQUIRED)

set(CORE_FILES
            ModuleBase.cpp
            ModuleStore.cpp
            Output.cpp
            OptionMap.cpp
   )

set(CORE_HEADERS
            ModuleBase.h
            ModuleStore.h
   )

set(CORE_LIBS
            $<TARGET_OBJECTS:bpmodule_modulebase>
            $<TARGET_OBJECTS:bpmodule_python>
   )


ADD_MODULE(bpmodule_core "${CORE_FILES};${CORE_LIBS}" 
                         "" 
                         "${Boost_LIBRARIES};${PYTHON_LIBRARIES}"
                         "${Boost_INCLUDE_DIRS};${PYTHON_INCLUDE_DIRS}")

# Install library and python stuff
INSTALL_MODULE(bpmodule_core "")

# Install headers
install(FILES ${CORE_HEADERS} DESTINATION include/BPModule/core)
