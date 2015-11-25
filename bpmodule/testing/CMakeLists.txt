set(BPMODULE_TESTING_FILES
            TestModuleInfo.cpp
            TestDatastore.cpp
            TestOptions.cpp
            export.cpp
   )

set(BPMODULE_TESTING_HEADERS
            TestingBase.hpp
            TestConvert.hpp
            TestDatastore.hpp
            TestOptions.hpp
            TestModuleInfo.hpp
   )
  


ADD_SUPERMODULE(testing
           "${BPMODULE_TESTING_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(testing "${BPMODULE_TESTING_HEADERS}" "tester.py")
