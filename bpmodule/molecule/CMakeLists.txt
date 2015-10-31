set(BPMODULE_MOLECULE_FILES
    AtomicInfo.cpp
    AtomicInfo_LUT.cpp
    Molecule.cpp
    export.cpp
   )
  


ADD_MODULE(molecule
           "${BPMODULE_MOLECULE_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(molecule "")

