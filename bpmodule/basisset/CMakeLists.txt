set(BPMODULE_BASISSET_FILES
    BasisSet.cpp
    Creators.cpp
    AMConvert.cpp
    export.cpp
   )


ADD_COREMODULE(basisset
           "${BPMODULE_BASISSET_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(basisset)
