set(BPMODULE_UTIL_FILES
            StringUtil.cpp 
            FormatString.cpp
            export.cpp
   )


ADD_MODULE(util 
           "${BPMODULE_UTIL_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(util "")
