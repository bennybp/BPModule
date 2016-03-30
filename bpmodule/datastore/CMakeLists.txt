set(BPMODULE_DATASTORE_FILES
            CacheData.cpp
            OptionHolder.cpp
            OptionTypes.cpp
            OptionBase.cpp
            OptionMap.cpp
            export.cpp
   )

  
ADD_COREMODULE(datastore
           "${BPMODULE_DATASTORE_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(datastore)

