set(BPMODULE_DATASTORE_FILES
            UIDPointer.cpp
            RegisterUIDPointer.cpp
            OptionHolder.cpp
            OptionBase.cpp
            OptionMap.cpp
            ModuleGraph.cpp
            export.cpp
   )

  
ADD_COREMODULE(datastore
           "${BPMODULE_DATASTORE_FILES}"
           ""
           "taskforce"
           "")

INSTALL_COREMODULE(datastore)

