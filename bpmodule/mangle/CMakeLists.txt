set(BPMODULE_MANGLE_FILES
            Mangle.cpp
            export.cpp
   )


ADD_MODULE(mangle
           "${BPMODULE_MANGLE_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(mangle "")
