set(BPMODULE_EXCEPTION_FILES
            Exceptions.cpp
            export.cpp
   )

set(BPMODULE_EXCEPTION_HEADERS
            Assert.hpp
            Exceptions.hpp
   )


ADD_SUPERMODULE(exception
           "${BPMODULE_EXCEPTION_FILES}"
           ""
           ""
           "")


INSTALL_COREMODULE(exception "${BPMODULE_EXCEPTION_HEADERS}" "Exceptions.py")
