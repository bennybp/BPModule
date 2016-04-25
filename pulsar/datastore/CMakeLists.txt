set(PULSAR_DATASTORE_FILES
            OptionHolder.cpp
            OptionTypes.cpp
            OptionMap.cpp
            Wavefunction.cpp
            export.cpp
            testing_export.cpp
   )

  
ADD_COREMODULE(datastore
           "${PULSAR_DATASTORE_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(datastore)

