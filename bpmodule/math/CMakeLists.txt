set(BPMODULE_MATH_FILES
            Factorial_LUT.cpp
            WeightType.hpp
            WeightType.cpp
            Locus.hpp
            Locus.cpp
            export.cpp
   )
  

ADD_COREMODULE(math
           "${BPMODULE_MATH_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(math)
