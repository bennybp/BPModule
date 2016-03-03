set(BPMODULE_MODULEBASE_FILES
            Method.cpp
            ModuleBase.cpp
            export.cpp
   )


ADD_COREMODULE(modulebase
           "${BPMODULE_MODULEBASE_FILES}"
           ""
           "taskforce"
           "${LIBTASKFORCE_INCLUDE_DIRS}")

INSTALL_COREMODULE(modulebase)
