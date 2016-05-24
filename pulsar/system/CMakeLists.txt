set(SYM_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/symmetry)

set(PULSAR_SYSTEM_FILES
    AtomicInfo.cpp
    AtomicInfo_LUT.cpp
    Atom.cpp
    ${SYM_FOLDER}/CrystalSystem.cpp
    ${SYM_FOLDER}/Symmetrizer.cpp
    ${SYM_FOLDER}/SymmetryElement.cpp
    ${SYM_FOLDER}/SymmetryGroup.cpp
    ${SYM_FOLDER}/ExportSymmetry.cpp
    CrystalFunctions.cpp
    System.cpp
    BasisSet.cpp
    AMConvert.cpp
    AOOrdering_LUT.cpp
    AOOrdering.cpp
    Space.cpp
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
