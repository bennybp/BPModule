set(BPMODULE_UTIL_FILES
            StringUtil.cpp 
            FormatString.cpp
            export.cpp
   )

set(BPMODULE_UTIL_HEADERS
            FormatString.hpp
            StringUtil.hpp
   )


ADD_MODULE(util 
           "${BPMODULE_UTIL_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(util "${BPMODULE_UTIL_HEADERS}" "")
