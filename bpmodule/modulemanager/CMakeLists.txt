set(BPMODULE_MODULEMANAGER_FILES
            CppSupermoduleLoader.cpp
            PySupermoduleLoader.cpp
            ModuleIMPLHolder.cpp
            ModuleInfo.cpp
            ModuleManager.cpp
            export.cpp
   )


ADD_COREMODULE(modulemanager
           "${BPMODULE_MODULEMANAGER_FILES}"
           ""
           "taskforce"
           "")

INSTALL_COREMODULE(modulemanager)

