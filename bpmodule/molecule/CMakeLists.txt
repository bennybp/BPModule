set(BPMODULE_MOLECULE_FILES
    AtomicInfo.cpp
    AtomicInfo_LUT.cpp
    Atom.cpp
    Molecule.cpp
    export.cpp
   )
  
set(BPMODULE_MOLECULE_HEADERS
            AtomicInfo.hpp
            AtomicInfo_LUT.hpp
            Molecule.hpp
            Atom.hpp
   )


ADD_MODULE(molecule
           "${BPMODULE_MOLECULE_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(molecule "${BPMODULE_MOLECULE_HEADERS}" "")

