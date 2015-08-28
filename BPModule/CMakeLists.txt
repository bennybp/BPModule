##############################
# Add core library directories
##############################
add_subdirectory(core)
add_subdirectory(python_helper)
add_subdirectory(modulebase)
add_subdirectory(parallel)
add_subdirectory(tensor)


##############################
# Create the core SO file
##############################
set(CORE_LIB
            export_base.cpp
            $<TARGET_OBJECTS:bpmodule_core>
            $<TARGET_OBJECTS:bpmodule_modulebase>
            $<TARGET_OBJECTS:bpmodule_parallel>
            $<TARGET_OBJECTS:bpmodule_tensor>
            $<TARGET_OBJECTS:bpmodule_python_helper>
            ${BPMODULE_CYCLOPS_PATH}/lib/libctf.a
   )


ADD_MODULE(bpmodule_base "${CORE_LIB}"
                         ""
                         "${Boost_LIBRARIES};${PYTHON_LIBRARIES}"
                         "${Boost_INCLUDE_DIRS};${PYTHON_INCLUDE_DIRS}")


# Install library and python stuff
INSTALL_MODULE(bpmodule_base "")


# Install headers
set(CORE_HEADERS
            core/CalcData.hpp
            core/CModuleLoader.hpp
            core/Exception.hpp
            core/ModuleBase.hpp
            core/ModuleInfo.hpp
            core/ModuleStore.hpp
            core/OptionMap.hpp
            core/Output.hpp
            core/PropertyMap.hpp
            core/PyModuleLoader.hpp
   )


set(TENSOR_HEADERS
            tensor/Tensor.hpp
   )


set(PARALLEL_HEADERS
            parallel/Parallel.hpp
   )


set(PYTHON_HEADERS
            python_helper/BoostPython_fwd.hpp
            python_helper/Python_stdconvert.hpp
   )


install(FILES ${CORE_HEADERS} DESTINATION include/BPModule/core)
install(FILES ${PYTHON_HEADERS} DESTINATION include/BPModule/python_helper)
install(FILES ${PARALLEL_HEADERS} DESTINATION include/BPModule/parallel)
install(FILES ${TENSOR_HEADERS} DESTINATION include/BPModule/tensor)
