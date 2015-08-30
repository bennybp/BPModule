set(BPMODULE_MODULEBASE_FILES
            ModuleBase.cpp
            Test_Base.cpp
            export.cpp
   )


ADD_MODULE(bpmodule_modulebase
           "${BPMODULE_MODULEBASE_FILES}"
           ""
           ""
           "")

INSTALL_MODULE(bpmodule_modulebase "")

