set(BPMODULE_MATH_FILES
            Factorial_LUT.cpp
            export.cpp
   )
  
set(BPMODULE_MATH_HEADERS
            MinMax.hpp
            Cast.hpp
            Cast_stl.hpp
            ExactCast.hpp
            Factorial.hpp
            Factorial_LUT.hpp
   )


ADD_SUPERMODULE(math
           "${BPMODULE_MATH_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(math "${BPMODULE_MATH_HEADERS}" "")
