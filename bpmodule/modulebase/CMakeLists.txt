set(BPMODULE_MODULEBASE_FILES
            ModuleBase.cpp
            Test_Base.cpp
            export.cpp
   )


ADD_MODULE(modulebase
           "${BPMODULE_MODULEBASE_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(modulebase "")

