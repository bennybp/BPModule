set(BPMODULE_MODULEBASE_FILES
            ModuleBase.cpp
            OneElectronIntegralIMPL.cpp
            TwoElectronIntegralIMPL.cpp
            export.cpp
   )

set(BPMODULE_MODULEBASE_HEADERS
            All.hpp
            ModuleBase.hpp
            Test_Base.hpp
            OneElectronIntegralIMPL.hpp
            TwoElectronIntegralIMPL.hpp
   )


ADD_SUPERMODULE(modulebase
           "${BPMODULE_MODULEBASE_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(modulebase "${BPMODULE_MODULEBASE_HEADERS}" "")

