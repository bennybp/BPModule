set(BPMODULE_MANGLE_FILES
            Mangle.cpp
            export.cpp
   )

set(BPMODULE_MANGLE_HEADERS
            Mangle.hpp
)


ADD_MODULE(mangle
           "${BPMODULE_MANGLE_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(mangle "${BPMODULE_MANGLE_HEADERS}" "")
