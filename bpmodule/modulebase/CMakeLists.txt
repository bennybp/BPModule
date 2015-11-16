set(BPMODULE_MODULEBASE_FILES
            ModuleBase.cpp
            Test_Base.cpp
            export.cpp
   )

set(BPMODULE_MODULEBASE_HEADERS
            ModuleBase.hpp
            Test_Base.hpp
   )


ADD_MODULE(modulebase
           "${BPMODULE_MODULEBASE_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(modulebase "${BPMODULE_MODULEBASE_HEADERS}" "")

