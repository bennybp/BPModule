set(BPMODULE_UTIL_FILES
            Cmdline.cpp
            StringUtil.cpp 
            FormatString.cpp
            export.cpp
   )

set(BPMODULE_UTIL_HEADERS
            Cmdline.hpp
            FormatString.hpp
            StringUtil.hpp
   )


ADD_SUPERMODULE(util 
           "${BPMODULE_UTIL_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(util "${BPMODULE_UTIL_HEADERS}" "")
