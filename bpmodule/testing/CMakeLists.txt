set(BPMODULE_TESTING_FILES
            TestModuleInfo.cpp
            TestDatastore.cpp
            TestOptions.cpp
            export.cpp
   )
  


ADD_MODULE(testing
           "${BPMODULE_TESTING_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(testing "tester.py")
