set(PULSAR_DATASTORE_FILES
            CacheData.cpp
            OptionHolder.cpp
            OptionTypes.cpp
            OptionBase.cpp
            OptionMap.cpp
            export.cpp
            testing_export.cpp
   )

  
ADD_COREMODULE(datastore
           "${PULSAR_DATASTORE_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(datastore)

