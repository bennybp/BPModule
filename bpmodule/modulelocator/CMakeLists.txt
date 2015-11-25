set(BPMODULE_MODULELOCATOR_FILES
            CModuleLoader.cpp
            export.cpp
            ModuleInfo.cpp
            ModuleLocator.cpp
            PyModuleLoader.cpp
            export.cpp
   )

set(BPMODULE_MODULELOCATOR_HEADERS
            ScopedModule.hpp
            CModuleLoader.hpp
            ModuleInfo.hpp
            ModuleLocator.hpp
            PyModuleLoader.hpp
            Graph.hpp
   )


ADD_SUPERMODULE(modulelocator
           "${BPMODULE_MODULELOCATOR_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(modulelocator "${BPMODULE_MODULELOCATOR_HEADERS}" "")
