set(BPMODULE_TESTING_FILES
            TestModuleInfo.cpp
            export.cpp
   )
  


ADD_MODULE(testing
           "${BPMODULE_TESTING_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(testing "")
