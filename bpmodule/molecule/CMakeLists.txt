set(BPMODULE_MOLECULE_FILES
    AtomicInfo.cpp
    AtomicInfo_LUT.cpp
    Atom.cpp
    Molecule.cpp
    export.cpp
   )
  

ADD_COREMODULE(molecule
           "${BPMODULE_MOLECULE_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(molecule)
