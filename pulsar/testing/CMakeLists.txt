set(PULSAR_TESTING_FILES
            TestDatastore.cpp
            TestOptions.cpp
            export.cpp
   )


ADD_COREMODULE(testing
           "${PULSAR_TESTING_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(testing)
