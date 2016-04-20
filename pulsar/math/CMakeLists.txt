set(PULSAR_MATH_FILES
            Binomial.cpp
            Factorial_LUT.cpp
            FiniteDiff.cpp
            SimpleMatrix.cpp
            IrrepSpinMatrix.cpp
            export.cpp
            testing_export.cpp
   )
  

ADD_COREMODULE(math
           "${PULSAR_MATH_FILES}"
           ""
           "${PULSAR_MATH_LIBS}"
           "")
INSTALL_COREMODULE(math)
