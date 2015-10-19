set(BPMODULE_MATH_FILES
            Factorial_LUT.cpp
            export.cpp
   )
  


ADD_MODULE(math
           "${BPMODULE_MATH_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(math "")
