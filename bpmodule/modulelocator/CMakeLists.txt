set(BPMODULE_MODULELOCATOR_FILES
            CppModuleLoader.cpp
            ModuleIMPLHolder.cpp
            ModuleInfo.cpp
            ModuleLocator.cpp
            export.cpp
   )

set(BPMODULE_MODULELOCATOR_HEADERS
            CppModuleLoader.hpp
            ModuleInfo.hpp
            ModuleLocator.hpp
            ModulePtr.hpp
            ModuleIMPLHolder.hpp
            ModuleCreationFuncs.hpp
   )


ADD_SUPERMODULE(modulelocator
           "${BPMODULE_MODULELOCATOR_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(modulelocator "${BPMODULE_MODULELOCATOR_HEADERS}" "PyModuleLoader.py")
