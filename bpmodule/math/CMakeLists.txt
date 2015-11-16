set(BPMODULE_MATH_FILES
            Factorial_LUT.cpp
            export.cpp
   )
  
set(BPMODULE_MATH_HEADERS
            Cast.hpp
            ExactCast.hpp
            Factorial.hpp
            Factorial_LUT.hpp
   )


ADD_MODULE(math
           "${BPMODULE_MATH_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(math "${BPMODULE_MATH_HEADERS}" "")
