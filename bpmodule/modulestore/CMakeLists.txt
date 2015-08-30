set(BPMODULE_MODULESTORE_FILES
            CModuleLoader.cpp
            CModuleLoader_python.cpp
            export.cpp
            ModuleInfo_python.cpp
            ModuleStore.cpp
            ModuleStore_python.cpp
            PyModuleLoader.cpp
            export.cpp
   )
  


ADD_MODULE(modulestore
           "${BPMODULE_MODULESTORE_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(modulestore "")
