set(BPMODULE_TESTING_FILES
            TestDatastore.cpp
            TestOptions.cpp
            export.cpp
   )


ADD_SUPERMODULE(testing
           "${BPMODULE_TESTING_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(testing)
