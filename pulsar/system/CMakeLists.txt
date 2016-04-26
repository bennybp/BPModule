set(PULSAR_SYSTEM_FILES
    AtomicInfo.cpp
    AtomicInfo_LUT.cpp
    Atom.hpp
    Atom.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/symmetry/Symmetrizer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/symmetry/SymmetryElements.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/symmetry/SymmetryGroup.cpp
    System.cpp
    BasisSet.cpp
    AMConvert.cpp
    AOOrdering_LUT.cpp
    AOOrdering.cpp
    SphericalTransform.cpp
    SphericalTransform_LUT.cpp
    export.cpp
    testing_export.cpp
   )
  

ADD_COREMODULE(system
           "${PULSAR_SYSTEM_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(system)
