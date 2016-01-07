set(BPMODULE_PYTHON_HELPER_FILES
        Types.cpp
        Errors.cpp
        export.cpp
   )


ADD_SUPERMODULE(python
           "${BPMODULE_PYTHON_HELPER_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(python)
