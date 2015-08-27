##################################################
# Actual library
##################################################

set(CORE_FILES
            ModuleBase.cpp
            ModuleStore.cpp
            CModuleLoader.cpp
            PyModuleLoader.cpp
            Output.cpp
            PropertyMap.cpp

            PropertyMap_python.cpp
            ModuleInfo_python.cpp
   )

set(CORE_HEADERS
        CalcData.hpp
        Exception.hpp
        ModuleBase.hpp
        ModuleInfo.hpp
        OptionMap.hpp
        Output.hpp
        PropertyMap.hpp
        BoostPython_fwd.hpp
        ModuleStore.hpp
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
