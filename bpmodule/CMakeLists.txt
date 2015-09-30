add_subdirectory(python_helper)
add_subdirectory(output)
add_subdirectory(exception)
add_subdirectory(parallel)
add_subdirectory(tensor)
add_subdirectory(datastore)
add_subdirectory(options)
add_subdirectory(modulebase)
add_subdirectory(modulestore)
add_subdirectory(molecule)
add_subdirectory(basisset)
add_subdirectory(testing)


# Install headers
set(BPMODULE_PYTHON_HELPER_HEADERS
            python_helper/BoostPython_fwd.hpp
            python_helper/Types.hpp
            python_helper/Convert.hpp
   )

set(BPMODULE_OUTPUT_HEADERS
            output/Output.hpp
            output/FormatStr.hpp
   )

set(BPMODULE_EXCEPTION_HEADERS
            exception/GeneralException.hpp
            exception/PythonConvertException.hpp
            exception/OptionException.hpp
            exception/ModuleLoadException.hpp
            exception/ModuleCreateException.hpp
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
            datastore/PropHolder.hpp
            datastore/PropPlaceholder.hpp
   )

set(BPMODULE_OPTIONS_HEADERS
            options/OptionTypes.hpp
            options/OptionMap.hpp
            options/OptionBase.hpp
            options/OptionHolder.hpp
   )

set(BPMODULE_MODULEBASE_HEADERS
            modulebase/ModuleBase.hpp
            modulebase/Test_Base.hpp
   )

set(BPMODULE_MODULESTORE_HEADERS
            modulestore/ScopedModule.hpp
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


set(BPMODULE_TESTING_HEADERS
            testing/TestingBase.hpp
            testing/TestConvert.hpp
            testing/TestDatastore.hpp
            testing/TestOptions.hpp
            testing/TestModuleInfo.hpp
   )


install(FILES ${BPMODULE_PYTHON_HELPER_HEADERS} DESTINATION include/bpmodule/python_helper)
install(FILES ${BPMODULE_EXCEPTION_HEADERS} DESTINATION include/bpmodule/exception)
install(FILES ${BPMODULE_OUTPUT_HEADERS} DESTINATION include/bpmodule/output)
install(FILES ${BPMODULE_PARALLEL_HEADERS} DESTINATION include/bpmodule/parallel)
install(FILES ${BPMODULE_TENSOR_HEADERS} DESTINATION include/bpmodule/tensor)
install(FILES ${BPMODULE_DATASTORE_HEADERS} DESTINATION include/bpmodule/datastore)
install(FILES ${BPMODULE_OPTIONS_HEADERS} DESTINATION include/bpmodule/options)
install(FILES ${BPMODULE_MODULEBASE_HEADERS} DESTINATION include/bpmodule/modulebase)
install(FILES ${BPMODULE_MODULESTORE_HEADERS} DESTINATION include/bpmodule/modulestore)
install(FILES ${BPMODULE_MOLECULE_HEADERS} DESTINATION include/bpmodule/molecule)
install(FILES ${BPMODULE_BASISSET_HEADERS} DESTINATION include/bpmodule/basisset)
install(FILES ${BPMODULE_TESTING_HEADERS} DESTINATION include/testing/basisset)

set(BPMODULE_FILES
            __init__.py
            modinfo.py
            optioncheck.py
            optionvalidate.py
            utils.py
   )
install(FILES ${BPMODULE_FILES} DESTINATION modules/bpmodule)
