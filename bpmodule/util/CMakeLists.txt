set(BPMODULE_UTIL_FILES
            Memwatch.cpp
            Cmdline.cpp
            StringUtil.cpp 
            FormatString.cpp
            Mangle.cpp
            export.cpp
   )

set(BPMODULE_UTIL_HEADERS
            Memwatch.hpp
            Cmdline.hpp
            FormatString.hpp
            StringUtil.hpp
            Mangle.hpp
   )


ADD_SUPERMODULE(util 
           "${BPMODULE_UTIL_FILES}"
           ""
           "memwatch"
           "")

set_target_properties(util PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/memwatch")

INSTALL_COREMODULE(util "${BPMODULE_UTIL_HEADERS}" "")
