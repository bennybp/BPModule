set(BPMODULE_DATASTORE_FILES
            UIDPointer.cpp
            RegisterUIDPointer.cpp
            Wavefunction.cpp
            export.cpp
   )
  
set(BPMODULE_DATASTORE_HEADERS
            UIDPointer.hpp
            RegisterUIDPointer.hpp
            Wavefunction.hpp
            GraphNodeData.hpp
            CalcData.hpp
            PropertyMap.hpp
            GenericHolder.hpp
            GenericBase.hpp
   )


ADD_SUPERMODULE(datastore
           "${BPMODULE_DATASTORE_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(datastore "${BPMODULE_DATASTORE_HEADERS}" "")
