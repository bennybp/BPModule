set(BPMODULE_EXCEPTION_FILES
            Exceptions.cpp
            export.cpp
   )


ADD_SUPERMODULE(exception
           "${BPMODULE_EXCEPTION_FILES}"
           ""
           ""
           "")


INSTALL_COREMODULE(exception)
