set(BPMODULE_MODULELOCATOR_FILES
            CppModuleLoader.cpp
            ModuleIMPLHolder.cpp
            ModuleInfo.cpp
            ModuleLocator.cpp
            export.cpp
   )


ADD_SUPERMODULE(modulelocator
           "${BPMODULE_MODULELOCATOR_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(modulelocator)
