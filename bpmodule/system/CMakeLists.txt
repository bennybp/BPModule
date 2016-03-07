set(BPMODULE_SYSTEM_FILES
    AtomicInfo.cpp
    AtomicInfo_LUT.cpp
    Atom.hpp
    Atom.cpp
    Molecule.hpp
    Molecule.cpp

    BasisSet.cpp
    BasisShellInfo.cpp
    BasisSetShell.cpp
    AMConvert.cpp

    export.cpp
   )
  

ADD_COREMODULE(system
           "${BPMODULE_SYSTEM_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(system)
