set(BPMODULE_MODULEBASE_FILES
            ModuleBase.cpp
            Test_Base.cpp
            export.cpp
   )

set(BPMODULE_MODULEBASE_HEADERS
            All.hpp
            ModuleBase.hpp
            Test_Base.hpp
   )


ADD_SUPERMODULE(modulebase
           "${BPMODULE_MODULEBASE_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(modulebase "${BPMODULE_MODULEBASE_HEADERS}" "")

