##################################################
# Actual library
##################################################

set(CORE_FILES
            CModuleLoader.cpp
            Exception.cpp
            Exception_python.cpp
            ModuleInfo_python.cpp
            ModuleStore_python.cpp
            Output_python.cpp
            PropertyMap_python.cpp
            CModuleLoader_python.cpp
            ModuleBase.cpp
            ModuleStore.cpp
            Output.cpp 
            PropertyMap.cpp
            PyModuleLoader.cpp
   )


ADD_CORELIB(bpmodule_core "${CORE_FILES}"
                          ""
                          "${Boost_INCLUDE_DIRS};${PYTHON_INCLUDE_DIRS}")
