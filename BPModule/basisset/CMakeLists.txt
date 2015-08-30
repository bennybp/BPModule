set(BPMODULE_BASISSET_FILES
    BasisSet.cpp
   )
  


ADD_MODULE(bpmodule_basisset
           "${BPMODULE_BASISSET_FILES}"
           ""
           ""
           "")

INSTALL_MODULE(bpmodule_basisset "")
