set(PULSAR_MODULEMANAGER_FILES
            CppSupermoduleLoader.cpp
            PySupermoduleLoader.cpp
            ModuleIMPLHolder.cpp
            ModuleInfo.cpp
            ModuleManager.cpp
            ModuleTree.cpp
            ModuleTree_iterators.cpp
            export.cpp
            testing_export.cpp
   )


ADD_COREMODULE(modulemanager
           "${PULSAR_MODULEMANAGER_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(modulemanager)

