set(BPMODULE_BASISSET_FILES
    BasisSet.cpp
   )
  


ADD_MODULE(basisset
           "${BPMODULE_BASISSET_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(basisset "")
