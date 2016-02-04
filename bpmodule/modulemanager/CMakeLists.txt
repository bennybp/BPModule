set(BPMODULE_MODULEMANAGER_FILES
            CppModuleLoader.cpp
            PyModuleLoader.cpp
            ModuleIMPLHolder.cpp
            ModuleInfo.cpp
            ModuleManager.cpp
            export.cpp
   )


ADD_COREMODULE(modulemanager
           "${BPMODULE_MODULEMANAGER_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(modulemanager)
