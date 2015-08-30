set(BPMODULE_EXCEPTION_FILES
            Exception.cpp 
            Exception_python.cpp
            export.cpp
   )


ADD_MODULE(bpmodule_exception 
           "${BPMODULE_EXCEPTION_FILES}"
           ""
           ""
           "")

INSTALL_MODULE(bpmodule_exception "")
