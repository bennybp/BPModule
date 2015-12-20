set(BPMODULE_MODULELOCATOR_FILES
            CppModuleLoader.cpp
            export.cpp
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
            CreatorFunctions.hpp
   )


ADD_SUPERMODULE(modulelocator
           "${BPMODULE_MODULELOCATOR_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(modulelocator "${BPMODULE_MODULELOCATOR_HEADERS}" "PyModuleLoader.py")
