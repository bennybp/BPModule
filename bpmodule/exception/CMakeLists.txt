set(BPMODULE_EXCEPTION_FILES
            GeneralException.cpp 
            export.cpp
   )


ADD_MODULE(exception 
           "${BPMODULE_EXCEPTION_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(exception "PyGeneralException.py")
