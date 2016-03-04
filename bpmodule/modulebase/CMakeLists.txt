set(BPMODULE_MODULEBASE_FILES
            Method.cpp
            ModuleBase.cpp
            export.cpp
   )


ADD_COREMODULE(modulebase
           "${BPMODULE_MODULEBASE_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(modulebase)
