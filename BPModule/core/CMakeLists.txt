##################################################
# Actual library
##################################################

find_package(Boost COMPONENTS python3 REQUIRED)

set(Python_ADDITIONAL_VERSIONS "3.3;3.4")
find_package(PythonLibs REQUIRED)

set(CORE_FILES
            ModuleBase.cpp
            ModuleStore.cpp
            Core_python.cpp
            base/Test_Base.cpp
            )


ADD_MODULE(bpmodule_core "${CORE_FILES}" "" "${Boost_LIBRARIES};${PYTHON_LIBRARIES}" "${Boost_INCLUDE_DIRS};${PYTHON_INCLUDE_DIRS}") 

