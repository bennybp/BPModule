set(BPMODULE_BASISSET_FILES
    BasisSet.cpp
    Creators.cpp
    AMConvert.cpp
    export.cpp
   )

set(BPMODULE_BASISSET_HEADERS
            NCartesian.hpp
            GaussianShell.hpp
            BasisSet.hpp
            AMConvert.hpp
            Creators.hpp
   )


ADD_SUPERMODULE(basisset
           "${BPMODULE_BASISSET_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(basisset "${BPMODULE_BASISSET_HEADERS}" "")
