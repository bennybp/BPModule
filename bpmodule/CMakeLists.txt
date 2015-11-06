add_subdirectory(mangle)
add_subdirectory(python_helper)
add_subdirectory(output)
add_subdirectory(util)
add_subdirectory(exception)
add_subdirectory(parallel)
add_subdirectory(math)
add_subdirectory(tensor)
add_subdirectory(datastore)
add_subdirectory(options)
add_subdirectory(modulebase)
add_subdirectory(modulelocator)
add_subdirectory(molecule)
add_subdirectory(basisset)
add_subdirectory(testing)


# Install headers
set(BPMODULE_TOP_HEADERS
            pragma.h
)

set(BPMODULE_MANGLE_HEADERS
            mangle/Mangle.hpp
)

set(BPMODULE_PYTHON_HELPER_HEADERS
            python_helper/BoostPython_fwd.hpp
            python_helper/Types.hpp
            python_helper/Convert.hpp
            python_helper/Errors.hpp
            python_helper/Call.hpp
   )

set(BPMODULE_OUTPUT_HEADERS
            output/Output.hpp
   )

set(BPMODULE_UTIL_HEADERS
            util/FormatString.hpp
            util/StringUtil.hpp
   )

set(BPMODULE_EXCEPTION_HEADERS
            exception/GeneralException.hpp
            exception/MathException.hpp
            exception/PythonConvertException.hpp
            exception/PythonCallException.hpp
            exception/OptionException.hpp
            exception/ModuleLoadException.hpp
            exception/ModuleCreateException.hpp
            exception/ModuleLocatorException.hpp
   )

set(BPMODULE_PARALLEL_HEADERS
            parallel/Parallel.hpp
   )

set(BPMODULE_MATH_HEADERS
            math/Cast.hpp
            math/ExactCast.hpp
            math/Factorial.hpp
            math/Factorial_LUT.hpp
   )

set(BPMODULE_TENSOR_HEADERS
            tensor/Tensor.hpp
   )

set(BPMODULE_DATASTORE_HEADERS
            datastore/CalcData.hpp
            datastore/PropertyMap.hpp
            datastore/GenericHolder.hpp
            datastore/GenericBase.hpp
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

set(BPMODULE_MODULELOCATOR_HEADERS
            modulelocator/ScopedModule.hpp
            modulelocator/CModuleLoader.hpp
            modulelocator/ModuleInfo.hpp
            modulelocator/ModuleLocator.hpp
            modulelocator/PyModuleLoader.hpp
            modulelocator/Graph.hpp
   )

set(BPMODULE_MOLECULE_HEADERS
            molecule/AtomicInfo.hpp
            molecule/AtomicInfo_LUT.hpp
            molecule/Molecule.hpp
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


install(FILES ${BPMODULE_TOP_HEADERS} DESTINATION include/bpmodule)
install(FILES ${BPMODULE_MANGLE_HEADERS} DESTINATION include/bpmodule/mangle)
install(FILES ${BPMODULE_PYTHON_HELPER_HEADERS} DESTINATION include/bpmodule/python_helper)
install(FILES ${BPMODULE_EXCEPTION_HEADERS} DESTINATION include/bpmodule/exception)
install(FILES ${BPMODULE_OUTPUT_HEADERS} DESTINATION include/bpmodule/output)
install(FILES ${BPMODULE_UTIL_HEADERS} DESTINATION include/bpmodule/util)
install(FILES ${BPMODULE_PARALLEL_HEADERS} DESTINATION include/bpmodule/parallel)
install(FILES ${BPMODULE_MATH_HEADERS} DESTINATION include/bpmodule/math)
install(FILES ${BPMODULE_TENSOR_HEADERS} DESTINATION include/bpmodule/tensor)
install(FILES ${BPMODULE_DATASTORE_HEADERS} DESTINATION include/bpmodule/datastore)
install(FILES ${BPMODULE_OPTIONS_HEADERS} DESTINATION include/bpmodule/options)
install(FILES ${BPMODULE_MODULEBASE_HEADERS} DESTINATION include/bpmodule/modulebase)
install(FILES ${BPMODULE_MODULELOCATOR_HEADERS} DESTINATION include/bpmodule/modulelocator)
install(FILES ${BPMODULE_MOLECULE_HEADERS} DESTINATION include/bpmodule/molecule)
install(FILES ${BPMODULE_BASISSET_HEADERS} DESTINATION include/bpmodule/basisset)
install(FILES ${BPMODULE_TESTING_HEADERS} DESTINATION include/testing/basisset)

set(BPMODULE_FILES
            __init__.py
            modinfo.py
            modulecheck.py
            ModuleManager.py
   )
install(FILES ${BPMODULE_FILES} DESTINATION modules/bpmodule)
