set(BPMODULE_MOLECULE_FILES
    Molecule.cpp
    Atoms.cpp
    export.cpp
   )
  


ADD_MODULE(molecule
           "${BPMODULE_MOLECULE_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(molecule "")

