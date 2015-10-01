set(BPMODULE_MATH_FILES
            export.cpp
   )
  


ADD_MODULE(math
           "${BPMODULE_MATH_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(math "")
