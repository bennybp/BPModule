set(PULSAR_SYSTEM_FILES
    AtomicInfo.cpp
    AtomicInfo_LUT.cpp
    Atom.hpp
    Atom.cpp
    Symmetrizer.cpp
    SymmetryElements.cpp
    System.cpp
    BasisSet.cpp
    BasisShellBase.cpp
    BasisShellInfo.cpp
    BasisSetShell.cpp
    AMConvert.cpp
    Ordering_LUT.cpp
    Ordering.cpp

    export.cpp
    testing_export.cpp
   )
  

ADD_COREMODULE(system
           "${PULSAR_SYSTEM_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(system)
