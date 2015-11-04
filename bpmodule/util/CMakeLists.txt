set(BPMODULE_UTIL_FILES
            StrUtil.cpp 
            FormatStr.cpp 
            export.cpp
   )


ADD_MODULE(util 
           "${BPMODULE_UTIL_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(util "")
