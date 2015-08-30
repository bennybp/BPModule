set(BPMODULE_MOLECULE_FILES
    Molecule.cpp
    Atoms.cpp
   )
  


ADD_MODULE(bpmodule_molecule
           "${BPMODULE_MOLECULE_FILES}"
           ""
           ""
           "")

INSTALL_MODULE(bpmodule_molecule "")

