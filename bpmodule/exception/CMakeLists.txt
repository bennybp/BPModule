set(BPMODULE_EXCEPTION_FILES
            Exceptions.cpp
            export.cpp
   )


ADD_COREMODULE(exception
           "${BPMODULE_EXCEPTION_FILES}"
           ""
           ""
           "")


INSTALL_COREMODULE(exception)
