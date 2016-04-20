set(PULSAR_PYTHON_HELPER_FILES
        Types.cpp
        Errors.cpp
        export.cpp
   )


ADD_COREMODULE(python
           "${PULSAR_PYTHON_HELPER_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(python)
