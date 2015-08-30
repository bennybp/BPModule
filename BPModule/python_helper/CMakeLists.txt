set(BPMODULE_PYTHON_HELPER_FILES
        export.cpp
   )


ADD_MODULE(bpmodule_python_helper 
           "${BPMODULE_PYTHON_HELPER_FILES}"
           ""
           ""
           "")

INSTALL_MODULE(bpmodule_python_helper "")
