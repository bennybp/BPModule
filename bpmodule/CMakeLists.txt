add_subdirectory(python)
add_subdirectory(output)
add_subdirectory(util)
add_subdirectory(exception)
add_subdirectory(parallel)
add_subdirectory(math)
add_subdirectory(tensor)
add_subdirectory(datastore)
add_subdirectory(modulebase)
add_subdirectory(modulelocator)
add_subdirectory(molecule)
add_subdirectory(basisset)
add_subdirectory(testing)


# Install headers and other files
set(BPMODULE_TOP_HEADERS
            pragma.h
)

set(BPMODULE_FILES
            __init__.py
            modinfo.py
            modulecheck.py
            ModuleManager.py
   )


install(FILES ${BPMODULE_TOP_HEADERS} DESTINATION include/bpmodule)
install(FILES ${BPMODULE_FILES} DESTINATION modules/bpmodule)
