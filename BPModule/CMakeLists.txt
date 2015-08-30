add_subdirectory(python_helper)
add_subdirectory(output)
add_subdirectory(exception)
add_subdirectory(parallel)
add_subdirectory(tensor)
add_subdirectory(datastore)
add_subdirectory(modulebase)
add_subdirectory(modulestore)
add_subdirectory(molecule)
add_subdirectory(basisset)


# Install headers
set(BPMODULE_PYTHON_HELPER_HEADERS
            python_helper/BoostPython_fwd.hpp
            python_helper/Python_stdconvert.hpp
   )

set(BPMODULE_OUTPUT_HEADERS
            output/Output.hpp
   )

set(BPMODULE_EXCEPTION_HEADERS
            exception/Exception.hpp
   )

set(BPMODULE_PARALLEL_HEADERS
            parallel/Parallel.hpp
   )

set(BPMODULE_TENSOR_HEADERS
            tensor/Tensor.hpp
   )

set(BPMODULE_DATASTORE_HEADERS
            datastore/CalcData.hpp
            datastore/PropertyMap.hpp
            datastore/OptionMap.hpp
   )

set(BPMODULE_MODULEBASE_HEADERS
            modulebase/ModuleBase.hpp
            modulebase/Test_Base.hpp
   )

set(BPMODULE_MODULESTORE_HEADERS
            modulestore/CModuleLoader.hpp
            modulestore/ModuleInfo.hpp
            modulestore/ModuleStore.hpp
            modulestore/PyModuleLoader.hpp
   )

set(BPMODULE_MOLECULE_HEADERS
            molecule/Molecule.hpp
            molecule/Atoms.hpp
   )

set(BPMODULE_BASISSET_HEADERS
            basisset/BasisSet.hpp
   )


install(FILES ${BPMODULE_PYTHON_HELPER_HEADERS} DESTINATION include/BPModule/python_helper)
install(FILES ${BPMODULE_EXCEPTION_HEADERS} DESTINATION include/BPModule/exception)
install(FILES ${BPMODULE_OUTPUT_HEADERS} DESTINATION include/BPModule/output)
install(FILES ${BPMODULE_PARALLEL_HEADERS} DESTINATION include/BPModule/parallel)
install(FILES ${BPMODULE_TENSOR_HEADERS} DESTINATION include/BPModule/tensor)
install(FILES ${BPMODULE_DATASTORE_HEADERS} DESTINATION include/BPModule/datastore)
install(FILES ${BPMODULE_MODULEBASE_HEADERS} DESTINATION include/BPModule/modulebase)
install(FILES ${BPMODULE_MODULESTORE_HEADERS} DESTINATION include/BPModule/modulestore)
install(FILES ${BPMODULE_MOLECULE_HEADERS} DESTINATION include/BPModule/molecule)
install(FILES ${BPMODULE_BASISSET_HEADERS} DESTINATION include/BPModule/basisset)
