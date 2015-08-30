set(BPMODULE_EXCEPTION_FILES
            GeneralException.cpp 
            GeneralException_python.cpp
            export.cpp
   )


ADD_MODULE(bpmodule_exception 
           "${BPMODULE_EXCEPTION_FILES}"
           ""
           ""
           "")

INSTALL_MODULE(bpmodule_exception "")
