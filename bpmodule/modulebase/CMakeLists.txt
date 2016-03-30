set(BPMODULE_MODULEBASE_FILES
            EnergyMethod.cpp
            ModuleBase.cpp
            SystemFragmenter.cpp
            export.cpp
   )


ADD_COREMODULE(modulebase
           "${BPMODULE_MODULEBASE_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(modulebase)
