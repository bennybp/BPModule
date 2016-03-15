set(BPMODULE_MATH_FILES
            Factorial_LUT.cpp
            FiniteDiff.cpp
            export.cpp
   )
  

ADD_COREMODULE(math
           "${BPMODULE_MATH_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(math)
