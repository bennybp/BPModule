set(BPMODULE_MATH_FILES
            Binomial.cpp
            Factorial_LUT.cpp
            FiniteDiff.cpp
            SimpleMatrix.cpp
            IrrepSpinMatrix.cpp
            export.cpp
            testing_export.cpp
   )
  

ADD_COREMODULE(math
           "${BPMODULE_MATH_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(math)
