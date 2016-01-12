set(BPMODULE_MODULEBASE_FILES
            ModuleBase.cpp
            export.cpp
   )


ADD_COREMODULE(modulebase
           "${BPMODULE_MODULEBASE_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(modulebase)
