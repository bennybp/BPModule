set(BPMODULE_MODULELOCATOR_FILES
            CppModuleLoader.cpp
            ModuleIMPLHolder.cpp
            ModuleInfo.cpp
            ModuleManager.cpp
            export.cpp
   )


ADD_COREMODULE(modulelocator
           "${BPMODULE_MODULELOCATOR_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(modulelocator)
