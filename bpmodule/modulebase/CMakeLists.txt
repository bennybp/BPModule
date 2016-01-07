set(BPMODULE_MODULEBASE_FILES
            ModuleBase.cpp
            export.cpp
   )


ADD_SUPERMODULE(modulebase
           "${BPMODULE_MODULEBASE_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(modulebase)
