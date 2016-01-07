set(BPMODULE_MATH_FILES
            Factorial_LUT.cpp
            export.cpp
   )
  

ADD_SUPERMODULE(math
           "${BPMODULE_MATH_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(math)
