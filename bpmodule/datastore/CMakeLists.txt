set(BPMODULE_DATASTORE_FILES
            UIDPointer.cpp
            RegisterUIDPointer.cpp
            Wavefunction.cpp
            OptionHolder.cpp
            OptionMap.cpp
            export.cpp
   )
  
set(BPMODULE_DATASTORE_HEADERS
            UIDPointer.hpp
            RegisterUIDPointer.hpp
            Wavefunction.hpp
            GraphNodeData.hpp
            CacheData.hpp
            GenericHolder.hpp
            GenericBase.hpp
            OptionMap.hpp
            OptionHolder.hpp
   )


ADD_SUPERMODULE(datastore
           "${BPMODULE_DATASTORE_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(datastore "${BPMODULE_DATASTORE_HEADERS}" "optionvalidate.py")
