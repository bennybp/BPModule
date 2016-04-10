set(BPMODULE_SYSTEM_FILES
    AtomicInfo.cpp
    AtomicInfo_LUT.cpp
    Atom.hpp
    Atom.cpp
    System.hpp
    System.cpp
    BasisSet.cpp
    BasisShellBase.cpp
    BasisShellInfo.cpp
    BasisSetShell.cpp
    AMConvert.cpp

    export.cpp
    testing_export.cpp
   )
  

ADD_COREMODULE(system
           "${BPMODULE_SYSTEM_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(system)
