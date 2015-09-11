set(BPMODULE_MODULESTORE_FILES
            CModuleLoader.cpp
            export.cpp
            ModuleInfo.cpp
            ModuleStore.cpp
            PyModuleLoader.cpp
            export.cpp
   )
  


ADD_MODULE(modulestore
           "${BPMODULE_MODULESTORE_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(modulestore "")
