set(BPMODULE_PYTHON_HELPER_FILES
        export.cpp
   )


ADD_MODULE(python_helper 
           "${BPMODULE_PYTHON_HELPER_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(python_helper "")
