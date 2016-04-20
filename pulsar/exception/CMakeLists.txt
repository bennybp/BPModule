set(PULSAR_EXCEPTION_FILES
            Exceptions.cpp
            export.cpp
   )


ADD_COREMODULE(exception
           "${PULSAR_EXCEPTION_FILES}"
           ""
           ""
           "")


INSTALL_COREMODULE(exception)
