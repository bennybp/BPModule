set(BPMODULE_MODULELOCATOR_FILES
            CModuleLoader.cpp
            export.cpp
            ModuleInfo.cpp
            ModuleLocator.cpp
            PyModuleLoader.cpp
            export.cpp
   )



ADD_MODULE(modulelocator
           "${BPMODULE_MODULELOCATOR_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(modulelocator "")
