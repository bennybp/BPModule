set(BPMODULE_DATASTORE_FILES
            UIDPointer.cpp
            RegisterUIDPointer.cpp
            OptionHolder.cpp
            OptionBase.cpp
            OptionMap.cpp
            ModuleGraph.cpp
            export.cpp
   )

  
ADD_SUPERMODULE(datastore
           "${BPMODULE_DATASTORE_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(datastore)
